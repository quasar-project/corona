#[derive(Copy, Clone, Debug, PartialOrd, PartialEq)]
pub struct Coordinate
{
    pub latitude: f64,
    pub longitude: f64,
    pub error_meters: f64
}

impl Coordinate
{
    pub fn new(lat: f64, lon: f64, error_meters: Option<f64>) -> Self
    {
        Self {
            latitude: lat,
            longitude: lon,
            error_meters: error_meters.unwrap_or(0.0)
        }
    }
}