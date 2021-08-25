/***************************************************************************************************
 USINGS
***************************************************************************************************/

use crate::ascii::*;

/***************************************************************************************************
 CONSTANTS
***************************************************************************************************/

pub const LOGO_COUNT: i32 = 3;

/***************************************************************************************************
 LOGOS
***************************************************************************************************/

pub const LOGO1_STR_1: &str =  "  /$$$$$$                                /$$$$$$$            /$$                                   ";
pub const LOGO1_STR_2: &str =  " /$$__  $$                              | $$__  $$          |__/                                   ";
pub const LOGO1_STR_3: &str =  "| $$  \\ $$  /$$$$$$   /$$$$$$  /$$$$$$$ | $$  \\ $$  /$$$$$$  /$$ /$$    /$$ /$$$$$$   /$$$$$$$   ";
pub const LOGO1_STR_4: &str =  "| $$  | $$ /$$__  $$ /$$__  $$| $$__  $$| $$  | $$ /$$__  $$| $$|  $$  /$$//$$__  $$ /$$_____/     ";
pub const LOGO1_STR_5: &str =  "| $$  | $$| $$  \\ $$| $$$$$$$$| $$  \\ $$| $$  | $$| $$  \\__/| $$ \\  $$/$$/| $$$$$$$$|  $$$$$$  ";
pub const LOGO1_STR_6: &str =  "| $$  | $$| $$  | $$| $$_____/| $$  | $$| $$  | $$| $$      | $$  \\  $$$/ | $$_____/ \\____  $$   ";
pub const LOGO1_STR_7: &str =  "|  $$$$$$/| $$$$$$$/|  $$$$$$$| $$  | $$| $$$$$$$/| $$      | $$   \\  $/  |  $$$$$$$ /$$$$$$$/    ";
pub const LOGO1_STR_8: &str =  " \\______/ | $$____/  \\_______/|__/  |__/|_______/ |__/      |__/    \\_/    \\_______/|_______/  ";
pub const LOGO1_STR_9: &str =  "          | $$                                                                                     ";
pub const LOGO1_STR_10: &str = "          | $$                                                                                     ";
pub const LOGO1_STR_11: &str = "          |__/                                                                                     ";

pub const LOGO2_STR_1: &str =  "    )                     (                                  ";
pub const LOGO2_STR_2: &str =  " ( /(                     )\\ )                              ";
pub const LOGO2_STR_3: &str =  " )\\())           (       (()/(   (   (    )      (          ";
pub const LOGO2_STR_4: &str =  "((_)\\   `  )    ))\\  (    /(_))  )(  )\\  /((    ))\\ (    ";
pub const LOGO2_STR_5: &str =  "  ((_)  /(/(   /((_) )\\ )(_))_  (()\\((_)(_))\\  /((_))\\   ";
pub const LOGO2_STR_6: &str =  " / _ \\ ((_)_\\ (_))  _(_/( |   \\  ((_)(_)_)((_)(_)) ((_)   ";
pub const LOGO2_STR_7: &str =  "| (_) || '_ \\)/ -_)| ' \\))| |) || '_|| |\\ V / / -_)(_-<   ";
pub const LOGO2_STR_8: &str =  " \\___/ | .__/ \\___||_||_| |___/ |_|  |_| \\_/  \\___|/__/  ";
pub const LOGO2_STR_9: &str =  "       |_|                                                   ";

pub const LOGO3_STR_1: &str =  "    ,-----.    .-------.     .-''-.  ,---.   .--. ______     .-------.   .-./`) ,---.  ,---.   .-''-.     .-'''-.             ";
pub const LOGO3_STR_2: &str =  "  .'  .-,  '.  \\  _(`)_ \\  .'_ _   \\ |    \\  |  ||    _ `''. |  _ _   \\  \\ .-.')|   /  |   | .'_ _   \\   / _     \\    ";
pub const LOGO3_STR_3: &str =  " / ,-.|  \\ _ \\ | (_ o._)| / ( ` )   '|  ,  \\ |  || _ | ) _  \\| ( ' )  |  / `-' \\|  |   |  .'/ ( ` )   ' (`' )/`--'       ";
pub const LOGO3_STR_4: &str =  ";  \\  '_ /  | :|  (_,_) /. (_ o _)  ||  |\\_ \\|  ||( ''_'  ) ||(_ o _) /   `-'`'`|  | _ |  |. (_ o _)  |(_ o _).            ";
pub const LOGO3_STR_5: &str =  "|  _`,/ \\ _/  ||   '-.-' |  (_,_)___||  _( )_\\  || . (_) `. || (_,_).' __ .---. |  _( )_  ||  (_,_)___| (_,_). '.           ";
pub const LOGO3_STR_6: &str =  ": (  '\\_/ \\   ;|   |     '  \\   .---.| (_ o _)  ||(_    ._) '|  |\\ \\  |  ||   | \\ (_ o._) /'  \\   .---..---.  \\  :    ";
pub const LOGO3_STR_7: &str =  " \\ `'/  \\  ) / |   |      \\  `-'    /|  (_,_)\\  ||  (_.\\.' / |  | \\ `'   /|   |  \\ (_,_) /  \\  `-'    /\\    `-'  |   ";
pub const LOGO3_STR_8: &str =  "  '. \\_/`'`.'  /   )       \\       / |  |    |  ||       .'  |  |  \\    / |   |   \\     /    \\       /  \\       /       ";
pub const LOGO3_STR_9: &str =  "    '-----'    `---'        `'-..-'  '--'    '--''-----'`    ''-'   `'-'  '---'    `---`      `'-..-'    `-...-'              ";

/***************************************************************************************************
 FUNCTIONS
***************************************************************************************************/

pub fn get_logo_as_ascii_art(number: i32) -> AsciiArt {
    match number {
        1 => {
            let mut result: AsciiArt = Default::default();
            result.data.push(LOGO1_STR_1.to_string());
            result.data.push(LOGO1_STR_2.to_string());
            result.data.push(LOGO1_STR_3.to_string());
            result.data.push(LOGO1_STR_4.to_string());
            result.data.push(LOGO1_STR_5.to_string());
            result.data.push(LOGO1_STR_6.to_string());
            result.data.push(LOGO1_STR_7.to_string());
            result.data.push(LOGO1_STR_8.to_string());
            result.data.push(LOGO1_STR_9.to_string());
            result.data.push(LOGO1_STR_10.to_string());
            result.data.push(LOGO1_STR_11.to_string());
            result.height = result.data.len() as i32;
            for s in &result.data {
                if result.length < s.len() as i32 {
                    result.length = s.len() as i32;
                }
            }
            result.anim_type = AnimationType::RightToLeft;
            result
        },
        2 => {
           let mut result: AsciiArt = Default::default();
            result.data.push(LOGO2_STR_1.to_string());
            result.data.push(LOGO2_STR_2.to_string());
            result.data.push(LOGO2_STR_3.to_string());
            result.data.push(LOGO2_STR_4.to_string());
            result.data.push(LOGO2_STR_5.to_string());
            result.data.push(LOGO2_STR_6.to_string());
            result.data.push(LOGO2_STR_7.to_string());
            result.data.push(LOGO2_STR_8.to_string());
            result.data.push(LOGO2_STR_9.to_string());
            result.height = result.data.len() as i32;
            for s in &result.data {
                if result.length < s.len() as i32 {
                    result.length = s.len() as i32;
                }
            }
            result.anim_type = AnimationType::UpperRightToLowerLeft;
            result
        },
        3 => {
            let mut result: AsciiArt = Default::default();
            result.data.push(LOGO3_STR_1.to_string());
            result.data.push(LOGO3_STR_2.to_string());
            result.data.push(LOGO3_STR_3.to_string());
            result.data.push(LOGO3_STR_4.to_string());
            result.data.push(LOGO3_STR_5.to_string());
            result.data.push(LOGO3_STR_6.to_string());
            result.data.push(LOGO3_STR_7.to_string());
            result.data.push(LOGO3_STR_8.to_string());
            result.data.push(LOGO3_STR_9.to_string());
            result.height = result.data.len() as i32;
            for s in &result.data {
                if result.length < s.len() as i32 {
                    result.length = s.len() as i32;
                }
            }
            result.anim_type = AnimationType::Shake;
            result
        },
        _ => {
            Default::default()
        }
    }
}






