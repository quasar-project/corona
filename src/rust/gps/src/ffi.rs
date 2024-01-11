#![allow(non_upper_case_globals, non_snake_case, non_camel_case_types, dead_code)]

use std::ffi::{
    c_char,
    c_double,
    c_int,
    CStr
};
use std::ptr::copy_nonoverlapping;
use std::slice;
use log::{
    error,
    info,
    warn
};
use std::cell::RefCell;
use anyhow::anyhow;
use crate::Coordinate;
use crate::logger::init_logger;
use crate::prelude::{
    current_location,
    LocationProvider
};

thread_local!
{
  static LAST_ERROR: RefCell<Option<Box<anyhow::Error>>> = RefCell::new(None);
}

#[repr(C)]
struct coordinate_t
{
    latitude: c_double,
    longitude: c_double,
    error_meters: c_double
}

impl From<Coordinate> for coordinate_t
{
    fn from(coord: Coordinate) -> Self
    {
        Self
        {
            latitude: coord.latitude,
            longitude: coord.longitude,
            error_meters: coord.error_meters
        }
    }
}

#[no_mangle]
unsafe fn QuaSAR_GPS_InitLogger(level: c_int) -> c_int
{
    let lvl = match level {
        -1 => "error",
        0 => "trace",
        1 => "debug",
        2 => "info",
        3 => "warn",
        4 => "error",
        _ => return -1
    };
    let init = init_logger(lvl);
    match init {
        Ok(_) => { info!("logger initialized"); 0 },
        Err(_) => { error!("failed to initialize logger"); -1 }
    }
}


#[no_mangle]
unsafe extern "C" fn QuaSAR_GPS_GetLastError(buffer: *mut c_char, length: c_int) -> c_int
{
    if buffer.is_null() {
        warn!("null pointer was passed into last_error as the buffer");
        return -1;
    }

    let last_err = match take_last_error() {
        Some(err) => err,
        None => {
            warn!("no error was set");
            return 0;
        }
    };

    let error_message = last_err.to_string();
    let buffer = slice::from_raw_parts_mut(buffer as *mut u8, length as usize);
    if error_message.len() >= buffer.len() {
        warn!("buffer provided for writing the last error message is too small!");
        warn!("expected at least {} bytes, got {}", error_message.len() + 1, buffer.len());
        return -1;
    }
    copy_nonoverlapping(error_message.as_ptr(), buffer.as_mut_ptr(), error_message.len());
    // trailing null
    buffer[error_message.len()] = 0;
    error_message.len() as c_int
}

#[no_mangle]
extern "C" fn QuaSAR_GPS_GetLastErrorLength() -> c_int
{
    LAST_ERROR.with(|prev| {
        match *prev.borrow() {
            Some(ref err) => err.to_string().len() as c_int + 1,
            None => 0
        }
    })
}

#[no_mangle]
unsafe extern "C" fn QuaSAR_GPS_GetLocalIp(buffer: *mut c_char, length: c_int) -> c_int
{
    if buffer.is_null() {
        warn!("null pointer was passed into local_ip as the buffer");
        return -1;
    }

    let ip = crate::network::local_ip();
    match ip {
        Ok(ip) => {
            let buffer = slice::from_raw_parts_mut(buffer as *mut u8, length as usize);
            if ip.len() >= buffer.len() {
                warn!("buffer provided for writing the local ip is too small!");
                warn!("expected at least {} bytes, got {}", ip.len() + 1, buffer.len());
                return -1;
            }
            copy_nonoverlapping(ip.as_ptr(), buffer.as_mut_ptr(), ip.len());
            // trailing null
            buffer[ip.len()] = 0;
            ip.len() as c_int
        },
        Err(err) => {
            update_last_error(err);
            -1
        }
    }
}

#[no_mangle]
unsafe extern "C" fn QuaSAR_GPS_GetPublicIp(buffer: *mut c_char, length: c_int) -> c_int
{
    if buffer.is_null() {
        warn!("null pointer was passed into public_ip as the buffer");
        return -1;
    }

    let ip = crate::network::public_ip_blocking();
    match ip {
        Ok(ip) => {
            let buffer = slice::from_raw_parts_mut(buffer as *mut u8, length as usize);
            if ip.len() >= buffer.len() {
                warn!("buffer provided for writing the public ip is too small!");
                warn!("expected at least {} bytes, got {}", ip.len() + 1, buffer.len());
                return -1;
            }
            copy_nonoverlapping(ip.as_ptr(), buffer.as_mut_ptr(), ip.len());
            // trailing null
            buffer[ip.len()] = 0;
            ip.len() as c_int
        },
        Err(err) => {
            update_last_error(err);
            -1
        }
    }
}

extern "C" fn QuaSAR_GPS_GetCurrentPosition(mode: c_int, ip: *const c_char) -> coordinate_t
{
    let mode = match mode {
        0 => LocationProvider::Gps,
        1 => LocationProvider::HttpsResolver,
        _ => {
            update_last_error(anyhow!("unknown location provider"));
            return coordinate_t { latitude: 0.0, longitude: 0.0, error_meters: 0.0 };
        }
    };
    let ip = unsafe {
        CStr::from_ptr(ip).to_str()
    };
    let ip = match ip {
        Ok(ip) => ip.to_string(),
        Err(err) => {
            update_last_error(anyhow!("failed to parse ip: {}", err));
            return coordinate_t { latitude: 0.0, longitude: 0.0, error_meters: 0.0 };
        }
    };
    let coordinate = match current_location(mode, Some(ip)) {
        Ok(x) => coordinate_t::from(x),
        Err(e) => {
            update_last_error(e);
            coordinate_t { latitude: 0.0, longitude: 0.0, error_meters: 0.0 }
        }
    };
    coordinate
}

fn update_last_error(error: anyhow::Error)
{
    error!("{}", error);
    LAST_ERROR.with(|last_error| {
        *last_error.borrow_mut() = Some(Box::new(error));
    })
}

fn take_last_error() -> Option<Box<anyhow::Error>>
{
    LAST_ERROR.with(|prev| prev.borrow_mut().take())
}

