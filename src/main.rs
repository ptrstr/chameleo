pub mod format;
pub mod steg;
use std::{error, fs, path::PathBuf};

use clap::{Parser, Subcommand};
use steg::{desteganograph, steganograph, BitPattern};

/// Versatile steganography tool
#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[command(subcommand)]
    command: Commands,

    #[arg(short, long)]
    active_bits: BitPattern,

    /// Source file to steganograph
    #[arg(short, long)]
    target: PathBuf,

    /// File to output to
    #[arg(short, long)]
    output: PathBuf,
}

#[derive(Subcommand)]
enum Commands {
    /// Steganograph input
    Encode {

        /// File to embed
        #[arg(short, long)]
        secret: PathBuf,

    },
    /// Desteganograph input
    Decode {
        /// Size of the secret data
        #[arg(short, long)]
        secret_size: usize,
    },
}

fn main() -> Result<(), Box<dyn error::Error>> {
    let args = Args::parse();

    match args.command {
        Commands::Encode { secret } => {
            let mut data = fs::read(args.target)?;
            let secret = fs::read(secret)?;

            steganograph(data.as_mut_slice(), secret.as_slice(), &args.active_bits)?;

            fs::write(args.output, data)?;

            Ok(())
        }
        Commands::Decode { secret_size } => {
            let mut data = fs::read(args.target)?;

            fs::write(
                args.output,
                desteganograph(
                    data.as_mut_slice(),
                    secret_size,
                    &args.active_bits
                )?,
            )?;

            Ok(())
        }
    }
}
