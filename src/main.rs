use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(short = 'i', long, help = "path to an input ASCII text file")]
    input_ascii: Option<std::path::PathBuf>,

    #[arg(short = 'o', long, help = "path to output Morse coded input")]
    output_ascii: Option<std::path::PathBuf>,

    #[arg(
        short = 'd',
        long,
        default_value_t = false,
        help = "disable Morse code audio player"
    )]
    disable_audio: bool,

    #[arg(
        short = 'p',
        long,
        default_value_t = false,
        help = "print Morse code to console"
    )]
    print_code: bool,

    #[arg(
        short = 'u',
        long,
        default_value_t = 50,
        value_parser = clap::value_parser!(u32).range(0..=100),
        help = "a integer percentage value, the higher the percentage the higher the pitch of each dot/dash"
    )]
    pitch: u32,

    #[arg(
        short = 'l',
        long,
        default_value_t = 25,
        value_parser = clap::value_parser!(u32).range(0..=100),
        help = "a integer percentage value, the higher the percentage the longer of each dot/dash tone lasts"
    )]
    duration: u32,
}

fn main() {
    let args = Args::parse();
    let config = morse::Config::new(
        args.input_ascii,
        args.output_ascii,
        args.disable_audio,
        args.print_code,
        args.pitch,
        args.duration,
    );
    if let Err(e) = morse::run(&config) {
        eprintln!("error: {}", e);
        std::process::exit(1);
    }
}
