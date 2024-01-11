use crate::Coordinate;

pub fn position_from_http(ip: &str) -> anyhow::Result<Coordinate>
{
    let info = geolocation::find(ip)?;
    let (lat, lon) = (info.latitude.parse::<f64>()?, info.longitude.parse::<f64>()?);
    Ok(Coordinate::new(lat, lon, None))
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_position_from_http()
    {
        let info = position_from_http("213.170.107.250").unwrap();
        println!("http position: {:?}", info);
        assert!(info.latitude > 59.0 && info.latitude < 60.0);
        assert!(info.longitude > 30.0 && info.longitude < 31.0);
    }
}