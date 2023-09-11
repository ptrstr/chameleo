use std::error;

pub trait Format {
    fn read(&self) -> Result<&[u8], Box<dyn error::Error>>;
    fn write(&mut self, data: &[u8]) -> Result<(), Box<dyn error::Error>>;

    fn encode(&self) -> Result<&[u8], Box<dyn error::Error>>;
}

pub trait FormatFactory {
    fn name(&self) -> &'static str;
    fn format(&self) -> &'static str;

    fn create(&self, path: &str) -> Box<dyn Format>;
}