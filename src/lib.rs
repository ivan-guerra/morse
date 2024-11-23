//! A Morse code converter and audio player library
//!
//! This library provides functionality to convert ASCII text to International Morse Code,
//! write the converted code to files, and play it as audio signals. It supports both
//! file-based and standard input/output operations.
use lazy_static::lazy_static;
use rodio::source::{SineWave, Source};
use rodio::{OutputStream, Sink};
use std::time::Duration;
use std::{collections::HashMap, io::Read};

/// Configuration struct for the Morse code converter
pub struct Config {
    /// Path to the input ASCII text file
    pub input_ascii: Option<std::path::PathBuf>,
    /// Path to the output ASCII file where Morse code will be written
    pub output_ascii: Option<std::path::PathBuf>,
    /// Flag to disable audio output
    pub disable_audio: bool,
    /// Flag to enable printing of Morse code to stdout
    pub print_code: bool,
    /// Audio player instance for Morse code playback
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

/// Audio playback configuration for Morse code signals
pub struct MorseCodeAudioPlayer {
    /// Frequency in Hz for the audio tone
    pitch: f32,
    /// Duration in seconds for a dot signal
    dot_duration: f32,
    /// Duration in seconds for a dash signal
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

    /// Plays the provided Morse code string as audio signals
    ///
    /// # Arguments
    ///
    /// * `morse` - A string containing Morse code symbols (., -, spaces, and /)
    ///
    /// # Returns
    ///
    /// * `Result<(), Box<dyn std::error::Error>>` - Ok if playback succeeds, Err otherwise
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

/// Reads text content from a file or stdin if no file path is provided
///
/// # Arguments
///
/// * `path` - Optional PathBuf containing the path to the input file
///
/// # Returns
///
/// * `Result<String, Box<dyn std::error::Error>>` - The content as a String if successful, or an error
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

/// Converts ASCII text to International Morse Code
///
/// # Arguments
///
/// * `ascii` - A string slice containing ASCII text to convert
///
/// # Returns
///
/// * `String` - The Morse code representation of the input text
///
/// # Rules
///
/// 1. Valid ASCII characters are converted to their Morse code equivalents
/// 2. Invalid characters are represented as '#'
/// 3. Characters within a word are separated by a single space
/// 4. Words are separated by ' / ' (forward slash with spaces)
pub fn ascii_to_morse(ascii: &str) -> String {
    ascii
        .split_whitespace()
        .map(|word| {
            word.chars()
                .map(|c| ASCII_MAP.get(&c.to_ascii_lowercase()).unwrap_or(&"#"))
                .cloned()
                .collect::<Vec<&str>>()
                .join(" ")
        })
        .collect::<Vec<String>>()
        .join(" / ")
}

/// Executes the Morse code conversion and playback process
///
/// # Arguments
///
/// * `config` - Reference to the Config struct containing runtime settings
///
/// # Returns
///
/// * `Result<(), Box<dyn std::error::Error>>` - Ok if all operations succeed, Err otherwise
///
/// # Operations
///
/// 1. Reads input text from file or stdin
/// 2. Converts ASCII text to Morse code
/// 3. Writes Morse code to output file if specified
/// 4. Prints Morse code to console if enabled
/// 5. Plays audio representation if not disabled
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

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use testdir::testdir;

    #[test]
    fn read_text_from_file() -> Result<(), Box<dyn std::error::Error>> {
        let dir = testdir!();
        let test_file = dir.join("test.txt");
        fs::write(&test_file, "test content")?;
        let path = Some(test_file);

        let result = read_text(&path)?;
        assert_eq!(result, "test content");
        Ok(())
    }

    #[test]
    fn read_text_from_stdin() -> Result<(), Box<dyn std::error::Error>> {
        // This test requires running in a separate process to simulate stdin
        // Using std::process::Command to test stdin input
        use std::process::Command;

        let output = Command::new("echo").arg("test input").output()?;

        let input = String::from_utf8(output.stdout)?;
        assert_eq!(input.trim(), "test input");
        Ok(())
    }

    #[test]
    fn read_text_missing_file() {
        let dir = testdir!();
        let path = Some(dir.join("nonexistent_file.txt"));
        let result = read_text(&path);
        assert!(result.is_err());
    }

    #[test]
    fn read_text_invalid_utf8() -> Result<(), Box<dyn std::error::Error>> {
        let dir = testdir!();
        let test_file = dir.join("invalid.txt");
        fs::write(&test_file, vec![0xFF, 0xFF, 0xFF])?;
        let path = Some(test_file);

        let result = read_text(&path);
        assert!(result.is_err());
        Ok(())
    }

    #[test]
    fn ascii_to_morse_single_letter() {
        assert_eq!(ascii_to_morse("A"), ".-");
        assert_eq!(ascii_to_morse("Z"), "--..");
    }

    #[test]
    fn ascii_to_morse_single_word() {
        assert_eq!(ascii_to_morse("SOS"), "... --- ...");
        assert_eq!(ascii_to_morse("HELLO"), ".... . .-.. .-.. ---");
    }

    #[test]
    fn ascii_to_morse_multiple_words() {
        assert_eq!(
            ascii_to_morse("HELLO WORLD"),
            ".... . .-.. .-.. --- / .-- --- .-. .-.. -.."
        );
    }

    #[test]
    fn ascii_to_morse_invalid_chars() {
        assert_eq!(ascii_to_morse("HI!"), ".... .. #");
        assert_eq!(ascii_to_morse("$12@3%"), "# .---- ..--- # ...-- #");
    }

    #[test]
    fn ascii_to_morse_mixed_case() {
        assert_eq!(ascii_to_morse("Hello"), ".... . .-.. .-.. ---");
        assert_eq!(ascii_to_morse("hELLo"), ".... . .-.. .-.. ---");
    }

    #[test]
    fn ascii_to_morse_extra_spaces() {
        assert_eq!(
            ascii_to_morse("SOS  HELP"),
            "... --- ... / .... . .-.. .--."
        );
        assert_eq!(ascii_to_morse("  HI  THERE  "), ".... .. / - .... . .-. .");
    }

    #[test]
    fn ascii_to_morse_empty_string() {
        assert_eq!(ascii_to_morse(""), "");
    }

    #[test]
    fn ascii_to_morse_all_invalid() {
        assert_eq!(ascii_to_morse("!@#$%"), "# # # # #");
    }
}
