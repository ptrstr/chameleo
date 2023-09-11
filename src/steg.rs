use std::{error, fmt, str::FromStr};

#[derive(Debug, Clone)]
struct NotEnoughBits;

impl fmt::Display for NotEnoughBits {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "not enough bits")
    }
}

impl error::Error for NotEnoughBits {}

#[derive(Clone, Debug)]
pub struct BitPattern {
    pattern: Vec<Vec<u8>>,
    size: usize,
}

impl BitPattern {
    pub fn new(raw_pattern: &[u8]) -> Self {
        let mut pattern = Vec::new();

        for p in raw_pattern {
            let mut cur_pat = Vec::new();

            for i in 0..u8::BITS {
                if (p >> i) & 1 == 1 {
                    cur_pat.push(i as u8);
                }
            }

            pattern.push(cur_pat);
        }

        let size = pattern.iter().map(Vec::len).sum();

        BitPattern {
            pattern: pattern,
            size,
        }
    }

    pub fn size(&self) -> usize {
        self.size
    }

    pub fn pattern(&self) -> &Vec<Vec<u8>> {
        &self.pattern
    }
}

impl FromStr for BitPattern {
    type Err = &'static str;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if s.len() % u8::BITS as usize != 0 {
            return Err("invalid size for bit pattern");
        }

        let mut raw_pattern = vec![0; s.len() / u8::BITS as usize];
        
        for i in 0..raw_pattern.len() {
            raw_pattern[i] = u8::from_str_radix(&s[i..], 2)
                .or(Err("non binary digits in bit pattern"))?;
        }

        Ok(BitPattern::new(raw_pattern.as_slice()))
    }
}

pub fn validate_length(
    data_size: usize,
    secret_size: usize,
    pattern: &BitPattern,
) -> Result<(), Box<dyn error::Error>> {
    if data_size * pattern.size() < secret_size * u8::BITS as usize {
        Err(Box::new(NotEnoughBits {}))
    } else {
        Ok(())
    }
}

pub fn steganograph(
    data: &mut [u8],
    secret: &[u8],
    pattern: &BitPattern,
) -> Result<(), Box<dyn error::Error>> {
    validate_length(data.len(), secret.len(), pattern)?;

    let mut pat_cycle = pattern.pattern().iter().cycle();
    let mut secret_i: usize = 0;

    'out: for data_item in data.iter_mut() {
        for pos in pat_cycle.next().unwrap() {
            if secret_i == secret.len() * u8::BITS as usize {
                break 'out;
            }

            let mask: u8 = !(1 << pos);
            let secret_bit = (secret[secret_i / u8::BITS as usize] >> (secret_i % u8::BITS as usize)) & 1;

            *data_item &= mask;
            *data_item |= secret_bit << pos;

            secret_i += 1;
        }
    }

    Ok(())
}

pub fn desteganograph(
    data: &[u8],
    secret_size: usize,
    pattern: &BitPattern,
) -> Result<Vec<u8>, Box<dyn error::Error>> {
    validate_length(data.len(), secret_size, pattern)?;

    let mut secret = vec![0; secret_size];

    let mut pat_cycle = pattern.pattern().iter().cycle();
    let mut secret_i: usize = 0;

    'out: for data_item in data {
        for pos in pat_cycle.next().unwrap() {
            if secret_i == secret.len() * u8::BITS as usize {
                break 'out;
            }

            let cur_bit = (data_item >> pos) & 1;

            secret[secret_i / u8::BITS as usize] |= cur_bit << (secret_i % u8::BITS as usize);

            secret_i += 1;
        }
    }

    Ok(secret)
}
