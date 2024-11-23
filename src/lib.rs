use lazy_static::lazy_static;
use rodio::source::{SineWave, Source};
use rodio::{OutputStream, Sink};
use std::time::Duration;
use std::{collections::HashMap, io::Read};

pub struct Config {
    pub input_ascii: Option<std::path::PathBuf>,
    pub output_ascii: Option<std::path::PathBuf>,
    pub disable_audio: bool,
    pub print_code: bool,
    pub audio: MorseCodeAudioPlayer,
}

impl Config {
    pub fn new(
        input_ascii: Option<std::path::PathBuf>,
        output_ascii: Option<std::path::PathBuf>,
        disable_audio: bool,
        print_code: bool,
        pitch: u8,
        duration: u8,
    ) -> Self {
        Self {
            input_ascii,
            output_ascii,
            disable_audio,
            print_code,
            audio: MorseCodeAudioPlayer::new(pitch, duration),
        }
    }
}

pub struct MorseCodeAudioPlayer {
    pitch: f32,
    dot_duration: f32,
    dash_duration: f32,
}

impl MorseCodeAudioPlayer {
    pub fn new(pitch: u8, duration: u8) -> Self {
        Self {
            pitch: f32::from(pitch),
            dot_duration: f32::from(duration),
            dash_duration: f32::from(duration) * 3.0,
        }
    }

    pub fn play(&self, morse: &str) -> Result<(), Box<dyn std::error::Error>> {
        const MIN_PITCH: f32 = 200.0; // Lowest frequency (A3)
        const MAX_PITCH: f32 = 800.0; // Highest frequency (A5)
        const MIN_DURATION: f32 = 0.050; // Minimum duration in seconds
        const MAX_DURATION: f32 = 0.250; // Maximum duration in seconds
        const PAUSE_SAMPLE_RATE: u32 = 44100;

        // Calculate frequency and duration based on user input
        let frequency = MIN_PITCH + (self.pitch / 100.0) * (MAX_PITCH - MIN_PITCH);
        let dot_duration_secs =
            MIN_DURATION + (self.dot_duration / 100.0) * (MAX_DURATION - MIN_DURATION);
        let dash_duration_secs =
            MIN_DURATION + (self.dash_duration / 100.0) * (MAX_DURATION - MIN_DURATION);

        // Initialize audio output stream and sink
        let (_stream, stream_handle) = OutputStream::try_default()?;
        let sink = Sink::try_new(&stream_handle)?;

        // Define pause sources
        let pause_config = (1, PAUSE_SAMPLE_RATE);
        let symbol_pause = rodio::source::Zero::<f32>::new(pause_config.0, pause_config.1)
            .take_duration(Duration::from_millis(self.dot_duration as u64));
        let char_pause = rodio::source::Zero::<f32>::new(pause_config.0, pause_config.1)
            .take_duration(Duration::from_millis(self.dot_duration as u64 * 3));
        let word_pause = rodio::source::Zero::<f32>::new(pause_config.0, pause_config.1)
            .take_duration(Duration::from_millis(self.dot_duration as u64 * 7));

        // Process morse code symbols
        for c in morse.chars() {
            match c {
                '.' => {
                    sink.append(
                        SineWave::new(frequency)
                            .take_duration(Duration::from_secs_f32(dot_duration_secs)),
                    );
                    sink.append(symbol_pause.clone());
                }
                '-' => {
                    sink.append(
                        SineWave::new(frequency)
                            .take_duration(Duration::from_secs_f32(dash_duration_secs)),
                    );
                    sink.append(symbol_pause.clone());
                }
                ' ' => sink.append(char_pause.clone()),
                '/' => sink.append(word_pause.clone()),
                _ => (),
            }
        }

        // Sleep until the end of the audio stream
        sink.sleep_until_end();
        Ok(())
    }
}

lazy_static! {
    static ref ASCII_MAP: HashMap<char, &'static str> = {
        let mut map = HashMap::new();
        map.insert('a', ".-");
        map.insert('b', "-...");
        map.insert('c', "-.-.");
        map.insert('d', "-..");
        map.insert('e', ".");
        map.insert('f', "..-.");
        map.insert('g', "--.");
        map.insert('h', "....");
        map.insert('i', "..");
        map.insert('j', ".---");
        map.insert('k', "-.-");
        map.insert('l', ".-..");
        map.insert('m', "--");
        map.insert('n', "-.");
        map.insert('o', "---");
        map.insert('p', ".--.");
        map.insert('q', "--.-");
        map.insert('r', ".-.");
        map.insert('s', "...");
        map.insert('t', "-");
        map.insert('u', "..-");
        map.insert('v', "...-");
        map.insert('w', ".--");
        map.insert('x', "-..-");
        map.insert('y', "-.--");
        map.insert('z', "--..");
        map.insert('0', "-----");
        map.insert('1', ".----");
        map.insert('2', "..---");
        map.insert('3', "...--");
        map.insert('4', "....-");
        map.insert('5', ".....");
        map.insert('6', "-....");
        map.insert('7', "--...");
        map.insert('8', "---..");
        map.insert('9', "----.");
        map
    };
}

fn read_text(path: &Option<std::path::PathBuf>) -> Result<String, Box<dyn std::error::Error>> {
    match path {
        Some(path) => Ok(std::fs::read_to_string(path)?),
        None => {
            let mut buffer = Vec::new();
            std::io::stdin().read_to_end(&mut buffer)?;
            Ok(String::from_utf8(buffer)?)
        }
    }
}

pub fn ascii_to_morse(ascii: &str) -> String {
    // 1. Translate valid input chars to their International Morse Code dot/dash representation.
    // 2. Display invalid input chars as #.
    // 3. Separate characters in a word with a single space.
    // 4. Separate words by a forward slash surrounded by single spaces.
    ascii
        .split_whitespace()
        .map(|word| {
            word.chars()
                .map(|c| ASCII_MAP.get(&c).unwrap_or(&"#"))
                .cloned()
                .collect::<Vec<&str>>()
                .join(" ")
        })
        .collect::<Vec<String>>()
        .join(" / ")
}

pub fn run(config: &Config) -> Result<(), Box<dyn std::error::Error>> {
    // Read ASCII text from file or stdin
    let text = read_text(&config.input_ascii)?;
    let morse = ascii_to_morse(&text);

    // Write Morse code to file if output path is provided
    if let Some(output_ascii) = &config.output_ascii {
        std::fs::write(output_ascii, &morse)?;
    }

    // Print Morse code to console if enabled
    if config.print_code {
        println!("{}", morse);
    }

    // Play Morse code audio if enabled
    if !config.disable_audio {
        config.audio.play(&morse)?;
    }

    Ok(())
}
