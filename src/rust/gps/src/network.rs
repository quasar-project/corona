use anyhow::Context;
use public_ip::http::HTTP_IPIFY_ORG;
use public_ip::Version;

pub fn local_ip() -> anyhow::Result<String>
{
    let ip = local_ip_address::local_ip()?;
    Ok(ip.to_string())
}

pub async fn public_ip() -> anyhow::Result<String>
{
    let ip = public_ip::addr_with(HTTP_IPIFY_ORG, Version::V4)
        .await
        .context("failed to get public ip address")?;
    Ok(ip.to_string())
}

#[tokio::main]
pub async fn public_ip_blocking() -> anyhow::Result<String>
{
    public_ip().await
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_local_ip()
    {
        let ip = local_ip().unwrap();
        assert!(!ip.is_empty());
        println!("local ip: {}", &ip);
    }

    #[test]
    fn test_public_ip()
    {
        let ip = public_ip_blocking().unwrap();
        assert!(!ip.is_empty());
        println!("public ip: {}", &ip);
    }
}