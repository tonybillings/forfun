/***************************************************************************************************
 USINGS
***************************************************************************************************/

use std::fmt;
use std::fs::File;
use crate::logo::*;
use crate::ascii::*;

/***************************************************************************************************
 CONSTANTS
***************************************************************************************************/

pub const VALID_OPTIONS: &str = "-n -f -t";
pub const INVALID_ARGS: &str = "Invalid argument(s) passed to sl.\r\n\r\nUSAGE:\r\n\r\nsl [OPTION_NAME] [OPTION_VALUE] ... \r\n\r\nOPTIONS:\r\n\r\n-n\tSpecify which animation to view (1 though 3).\r\n\r\n-f\tThe path to a file containing ASCII art; overrides the '-n' switch.\r\n\r\n-t\tPass in text (max 50 characters) to be converted to ASCII art; overrides the '-n' and '-f' switches.\r\n\r\n";
pub const INVALID_ARG_N: &str = "The value for parameter '-n' must be between 1 and 3.\r\n\r\n";
pub const INVALID_ARG_F: &str = "Error opening file: '{}'\n";
pub const INVALID_ARG_T: &str = "The text must not be greater than 50 characters.\n";

/***************************************************************************************************
 STRUCTS & TYPES
***************************************************************************************************/

#[derive(Default)]
pub struct CliArg {
    name: String,
    value: String
}

#[derive(Default)]
pub struct CliArgList {
    pub items: Vec<CliArg>
}

/***************************************************************************************************
 ERRORS
***************************************************************************************************/

#[derive(Default, Debug, Clone)]
pub struct InvalidCliArgsError {
    pub message: String
}

impl fmt::Display for InvalidCliArgsError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, stringify!(INVALID_ARGS))
    }
}

/***************************************************************************************************
 FUNCTIONS
***************************************************************************************************/

impl CliArgList {

    pub fn from_main(args: &Vec<String>) -> Result<CliArgList, InvalidCliArgsError> {
        if (args.len() - 1) % 2 != 0 {
            return Err(InvalidCliArgsError { message: INVALID_ARGS.to_string() });
        }

        let mut result: CliArgList = Default::default();
        let mut i = 1;

        while i < (args.len() - 1) {
            let arg: CliArg = CliArg {
                name: args.get(i).unwrap().to_string(),
                value: args.get(i+1).unwrap().to_string()
            };

            result.items.push(arg);
            i += 2;
        }

        let valid_options: Vec<&str> = VALID_OPTIONS.split(" ").collect();

        for arg in &result.items {
            if !valid_options.iter().any(|&o| String::from(o) == arg.name) {
                return Err(InvalidCliArgsError { message: INVALID_ARGS.to_string() });
            }

            match arg.name.as_str() {
                "-n" => {
                    let logo_num: i32 = arg.value.parse::<i32>().unwrap();

                    if logo_num < 1 || logo_num > LOGO_COUNT {
                        return Err(InvalidCliArgsError { message: INVALID_ARG_N.to_string() });
                    }
                },
                "-f" => {
                    let file_name = &arg.value;
                    if let Err(_) = File::open(file_name) {
                        return Err(InvalidCliArgsError { message: INVALID_ARG_F.replace("{}", file_name.as_str()).to_string() });
                    }
                },
                "-t" => {
                    let text = &arg.value;

                    if text.len() > MAX_TEXT_LENGTH as usize {
                        return Err(InvalidCliArgsError { message: INVALID_ARG_T.to_string() });
                    }
                },
               &_ => {} 
            }
        };

        Ok(result)
    }

    pub fn get(&mut self, name: &str) -> Option<&str> {
        for arg in self.items.iter() {
            if arg.name == String::from(name) {
                return Some(arg.value.as_str());
            }
        };

        return None;
    }
}