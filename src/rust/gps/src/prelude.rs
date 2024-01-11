use anyhow::{
    anyhow,
    Context
};
use crate::Coordinate;
use crate::http_location::position_from_http;

pub enum LocationProvider
{
    Gps,
    HttpsResolver
}

pub fn current_location(location_provider: LocationProvider, ip: Option<String>) -> anyhow::Result<Coordinate>
{
    match location_provider
    {
        LocationProvider::Gps => Err(anyhow!("not implemented")),
        LocationProvider::HttpsResolver => {
            position_from_http(ip.context("ip is required for https resolver")?.as_str())
        }
    }
}