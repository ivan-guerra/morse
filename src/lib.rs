use lazy_static::lazy_static;
use std::{collections::HashMap, io::Read};

pub struct Config {
    pub input_ascii: Option<std::path::PathBuf>,
    pub output_ascii: Option<std::path::PathBuf>,
    pub disable_audio: bool,
    pub print_code: bool,
    pub pitch: u32,
    pub duration: u32,
}

impl Config {
    pub fn new(
        input_ascii: Option<std::path::PathBuf>,
        output_ascii: Option<std::path::PathBuf>,
        disable_audio: bool,
        print_code: bool,
        pitch: u32,
        duration: u32,
    ) -> Self {
        Self {
            input_ascii,
            output_ascii,
            disable_audio,
            print_code,
            pitch,
            duration,
        }
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

fn write_text(path: &Option<std::path::PathBuf>, text: &str) -> Result<(), std::io::Error> {
    match path {
        Some(path) => std::fs::write(path, text),
        None => {
            println!("{}", text);
            Ok(())
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
    let text = read_text(&config.input_ascii)?;
    let morse = ascii_to_morse(&text);
    write_text(&config.output_ascii, &morse)?;

    Ok(())
}
