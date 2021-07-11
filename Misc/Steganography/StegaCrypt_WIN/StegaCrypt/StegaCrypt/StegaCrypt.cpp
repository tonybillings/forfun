// StegaCrypt v1.5
// Author: R. Anthony Billings
// 
// Used to store encrypted messages within bitmap images (steganography).  
// AES-128 is used for encryption.
//
// USAGE
//
// encrypt: stegacrypt -e INPUT_IMAGE OUTPUT_IMAGE KEY TEXT
// decrypt: stegacrypt -d INPUT_IMAGE KEY
//
// example: stegacrypt -e cat.bmp secret.bmp MyPassword "My secret message!"
// example: stegacrypt -d cat.bmp MyPassword
//
// ENCRYPT
// Input:  inputImage, outputImage, key, plainText, plainTextLength
//
// DECRYPT
// Input:  inputImage, key
// Output: plainText, plainTextLength
//
// FORMAT OF DATA EMBEDDED INTO PIXEL ARRAY
// |0---------------3|4----------------7|8----------n|
// | plainTextLength | cipherTextLength | cipherText |

#include "stdafx.h"
#include "stega.h"

using namespace stega;

void print_usage()
{
	cout << endl << "USAGE" << endl << endl
		<< "encrypt: stegacrypt -e INPUT_IMAGE OUTPUT_IMAGE KEY TEXT" << endl
		<< "decrypt: stegacrypt -d INPUT_IMAGE KEY" << endl
		<< endl
		<< "example: stegacrypt -e cat.bmp secret.bmp MyP@$$word! \"My secret text.\"" << endl
		<< "example: stegacrypt -d secret.bmp MyP@$$word!" << endl << endl
		<< "note: INPUT_IMAGE must be a 24-bit color bitmap (*.bmp) file" << endl;
}

void encrypt_mode(int argc, char* argv[])
{
	if (argc != 6)
	{
		print_usage();
		return;
	}

#ifdef DEBUG
	cout << "Mode: encrypt" << endl
		<< "Input image: " << argv[2] << endl
		<< "Output image: " << argv[3] << endl
		<< "Key: " << argv[4] << endl
		<< "Text: " << argv[5] << endl << endl;
#endif

	char* iImage = argv[2];
	char* oImage = argv[3];
	char* key = argv[4];
	char* text = argv[5];
	SRESULT result = stega_encrypt(iImage, oImage, padded_key(key), reinterpret_cast<const BYTE*>(text), get_str_len(text));

	switch (result)
	{
	case SRESULT_SUCESSS:
		cout << endl << "Encrypted message saved in '" << oImage << "'" << endl;
		break;
	case SRESULT_IMAGE_TOO_SMALL:
		cout << endl << "Image must be at least 10x10 pixels across and large enough to hold the encrypted message!" << endl << endl
			<< "To calculate the number of pixels required, use this formula:" << endl << "((plainTextLength + 16) & ~15) + 8" << endl;
		break;
	case SRESULT_FAILURE:
		cout << endl << "An error occurred!" << endl;
		break;
	}

	memset(key, 0, get_str_len(key));
	memset(text, 0, get_str_len(text));
}

void decrypt_mode(int argc, char* argv[])
{
	if (argc != 4)
	{
		print_usage();
		return;
	}

#ifdef DEBUG
	cout << "Mode: decrypt" << endl
		<< "Input image: " << argv[2] << endl
		<< "Key: " << argv[3] << endl << endl;
#endif

	char* iImage = argv[2];
	char* key = argv[3];
	unsigned char* plainText;
	SRESULT result = stega_decrypt(iImage, padded_key(key), &plainText);

	switch (result)
	{
	case SRESULT_SUCESSS:
		cout << endl << plainText << endl;
		memset(plainText, 0, get_str_len(plainText));
		delete[] plainText;
		break;
	case SRESULT_FAILURE:
		cout << endl << "An error occurred!" << endl;
		break;
	}

	memset(key, 0, get_str_len(key));
}

int main(int argc, char* argv[])
{
#ifdef DEBUG
	cout << "RUNNING DEBUG BUILD" << endl << endl;
#endif

	if (argc < 2)
	{
		print_usage();
		return 0;
	}

	string mode = argv[1];

	if (mode == "-e")
		encrypt_mode(argc, argv);
	else if (mode == "-d")
		decrypt_mode(argc, argv);

#ifdef DEBUG
	cin.get();
#endif

	return 0;
}
