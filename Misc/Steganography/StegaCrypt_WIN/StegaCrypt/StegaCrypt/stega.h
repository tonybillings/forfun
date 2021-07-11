#define SRESULT int
#define SRESULT_SUCESSS 0
#define SRESULT_FAILURE 1
#define SRESULT_IMAGE_TOO_SMALL 2

using namespace std;

namespace stega
{
	using namespace std;

	typedef unsigned char BIT;
	typedef unsigned char  BYTE;
	typedef unsigned short WORD;
	typedef unsigned long  DWORD;

	BYTE IV[] = { 22, 34, 51, 78, 9, 229, 105, 14, 1, 250, 139, 63, 183, 29, 155, 49 };

	void set_iv(const BYTE* key)
	{
		IV[0] = key[1] ^ key[2];
		IV[4] = key[5] ^ key[6];
		IV[9] = key[10] ^ key[11];
		IV[13] = key[14] ^ key[15];
		IV[15] = IV[0] ^ IV[4] ^ IV[9] ^ IV[13] ^ IV[15];
	}

	BYTE* padded_key(const char* key)
	{
		BYTE* bytes = new BYTE[16];
		memset(bytes, 0, 16);

		for (int i = 0; i < 16; i++)
		{
			if (key[i] == '\0') { break; }
			bytes[i] = key[i];
		}

		return bytes;
	}

	int aes_encrypt(const BYTE* key, const BYTE* plainText, DWORD ptLength, BYTE** cipherText, DWORD* ctLength)
	{
		*ctLength = (ptLength + 16) & ~15;
		*cipherText = new BYTE[*ctLength];

		if (register_cipher(&aes_desc) == -1)
		{
#ifdef DEBUG
			cout << endl << "Unable to register AES cipher" << endl;
#endif
			return 1;
		}

		int err;
		symmetric_CTR ctr;
		set_iv(key);
		if ((err = ctr_start(find_cipher("aes"), IV, key, 16, 0, CTR_COUNTER_LITTLE_ENDIAN | 4, &ctr)) != CRYPT_OK)
		{
#ifdef DEBUG
			cout << endl << error_to_string(err) << endl;
#endif
			return 1;
		}

		if ((err = ctr_encrypt(plainText, *cipherText, ptLength, &ctr) != CRYPT_OK))
		{
#ifdef DEBUG
			cout << endl << error_to_string(err) << endl;
#endif
			return 1;
		}

		ctr_done(&ctr);
		return 0;
	}

	int aes_decrypt(const BYTE* key, const BYTE* cipherText, DWORD ctLength, BYTE* plainText)
	{
		if (register_cipher(&aes_desc) == -1)
		{
#ifdef DEBUG
			cout << endl << "Unable to register AES cipher" << endl;
#endif
			return 1;
		}

		int err;
		symmetric_CTR ctr;
		set_iv(key);
		if ((err = ctr_start(find_cipher("aes"), IV, key, 16, 0, CTR_COUNTER_LITTLE_ENDIAN | 4, &ctr)) != CRYPT_OK)
		{
#ifdef DEBUG
			cout << endl << error_to_string(err) << endl;
#endif
			return 1;
		}

		if ((err = ctr_decrypt(cipherText, plainText, ctLength, &ctr) != CRYPT_OK))
		{
#ifdef DEBUG
			cout << endl << error_to_string(err) << endl;
#endif
			return 1;
		}

		ctr_done(&ctr);
		return 0;
	}

	struct FILE_INFO
	{
		DWORD width;
		DWORD height;
		DWORD size;
		DWORD size_padded;
		DWORD row_size;
		DWORD row_size_padded;
	};

	BYTE* get_bytes(DWORD value)
	{
		BYTE* bytes = new BYTE[4];
		bytes[0] = (BYTE)((value >> 24) & 0xFF);
		bytes[1] = (BYTE)((value >> 16) & 0xFF);
		bytes[2] = (BYTE)((value >> 8) & 0XFF);
		bytes[3] = (BYTE)((value & 0XFF));
		return bytes;
	}

	BIT* get_binary(BYTE byte)
	{
		BIT* binary = new BIT[8];
		binary[7] = (byte & 0x01) == 0x01;
		binary[6] = (byte & 0x02) == 0x02;
		binary[5] = (byte & 0x04) == 0x04;
		binary[4] = (byte & 0x08) == 0x08;
		binary[3] = (byte & 0x10) == 0x10;
		binary[2] = (byte & 0x20) == 0x20;
		binary[1] = (byte & 0x40) == 0x40;
		binary[0] = (byte & 0x80) == 0x80;
		return binary;
	}

	BYTE get_byte(BIT* binary)
	{
		BYTE byte = 0;
		if (binary[7] == 1) { byte |= 0x01; }
		if (binary[6] == 1) { byte |= 0x02; }
		if (binary[5] == 1) { byte |= 0x04; }
		if (binary[4] == 1) { byte |= 0x08; }
		if (binary[3] == 1) { byte |= 0x10; }
		if (binary[2] == 1) { byte |= 0x20; }
		if (binary[1] == 1) { byte |= 0x40; }
		if (binary[0] == 1) { byte |= 0x80; }
		return byte;
	}

	DWORD get_dword(BIT* binary)
	{
		DWORD dword = 0;
		if (binary[31] == 1) { dword |= 0x01; }
		if (binary[30] == 1) { dword |= 0x02; }
		if (binary[29] == 1) { dword |= 0x04; }
		if (binary[28] == 1) { dword |= 0x08; }
		if (binary[27] == 1) { dword |= 0x10; }
		if (binary[26] == 1) { dword |= 0x20; }
		if (binary[25] == 1) { dword |= 0x40; }
		if (binary[24] == 1) { dword |= 0x80; }

		if (binary[23] == 1) { dword |= 0x100; }
		if (binary[22] == 1) { dword |= 0x200; }
		if (binary[21] == 1) { dword |= 0x400; }
		if (binary[20] == 1) { dword |= 0x800; }
		if (binary[19] == 1) { dword |= 0x1000; }
		if (binary[18] == 1) { dword |= 0x2000; }
		if (binary[17] == 1) { dword |= 0x4000; }
		if (binary[16] == 1) { dword |= 0x8000; }

		if (binary[15] == 1) { dword |= 0x10000; }
		if (binary[14] == 1) { dword |= 0x20000; }
		if (binary[13] == 1) { dword |= 0x40000; }
		if (binary[12] == 1) { dword |= 0x80000; }
		if (binary[11] == 1) { dword |= 0x100000; }
		if (binary[10] == 1) { dword |= 0x200000; }
		if (binary[9] == 1) { dword |= 0x400000; }
		if (binary[8] == 1) { dword |= 0x800000; }

		if (binary[7] == 1) { dword |= 0x1000000; }
		if (binary[6] == 1) { dword |= 0x2000000; }
		if (binary[5] == 1) { dword |= 0x4000000; }
		if (binary[4] == 1) { dword |= 0x8000000; }
		if (binary[3] == 1) { dword |= 0x10000000; }
		if (binary[2] == 1) { dword |= 0x20000000; }
		if (binary[1] == 1) { dword |= 0x40000000; }
		if (binary[0] == 1) { dword |= 0x80000000; }
		return dword;
	}

	DWORD get_dword(BYTE* bytes, int startIndex)
	{
		return (DWORD)bytes[startIndex] << 24 | (DWORD)bytes[startIndex + 1] << 16 | (DWORD)bytes[startIndex + 2] << 8 | (DWORD)bytes[startIndex + 3];
	}

	DWORD get_dword_from_pixels(BYTE* pixelData, int startIndex)
	{
		startIndex *= 3;
		BIT* binary = new BIT[32];
		for (int i = 0; i < 12; i += 3)
		{
			int offset = (i / 3) * 8;
			BIT* bBinary = get_binary(pixelData[startIndex + i]);
			BIT* gBinary = get_binary(pixelData[startIndex + i + 1]);
			BIT* rBinary = get_binary(pixelData[startIndex + i + 2]);
			binary[0 + offset] = bBinary[5];
			binary[1 + offset] = bBinary[6];
			binary[2 + offset] = bBinary[7];
			binary[3 + offset] = gBinary[5];
			binary[4 + offset] = gBinary[6];
			binary[5 + offset] = gBinary[7];
			binary[6 + offset] = rBinary[5];
			binary[7 + offset] = rBinary[6];
			delete[] bBinary;
			delete[] gBinary;
			delete[] rBinary;
		}
		DWORD dword = get_dword(binary);
		delete[] binary;
		return dword;
	}

	FILE_INFO get_file_info(const char* filename)
	{
		FILE* file;
		fopen_s(&file, filename, "rb");
		BYTE* header = new BYTE[54];
		fread(header, sizeof(BYTE), 54, file);
		fclose(file);
		FILE_INFO info = {};
		info.width = *(int*)&header[18];
		info.height = *(int*)&header[22];
		info.size = info.width * info.height * 3;
		info.row_size = info.width * 3;
		info.row_size_padded = (info.row_size + 3) & (~3);
		info.size_padded = info.row_size_padded * info.height;
		delete[] header;
		return info;
	}

	void copy_header(const char* srcFilename, const char* destFilename, int headerLength)
	{
		FILE *srcF, *destF;
		fopen_s(&srcF, srcFilename, "rb");
		fopen_s(&destF, destFilename, "w+b");
		BYTE* header = new BYTE[headerLength];
		fread(header, sizeof(BYTE), headerLength, srcF);
		fwrite(header, sizeof(BYTE), headerLength, destF);
		delete[] header;
		fclose(srcF);
		fclose(destF);
	}

	void print_file_info(FILE_INFO info)
	{
		cout << "FILE INFO" << endl
			<< "Width: " << info.width << endl
			<< "Height: " << info.height << endl
			<< "Size: " << info.size << " Bytes" << endl
			<< "Size (padded): " << info.size_padded << " Bytes" << endl
			<< "Row size: " << info.row_size << " Bytes" << endl
			<< "Row size (padded): " << info.row_size_padded << " Bytes" << endl << endl;
	}

	void print_byte_array(const char* description, const BYTE* array, DWORD length)
	{
		cout << description << " (" << length << " Bytes):" << endl;
		for (unsigned int i = 0; i < length; i++)
			cout << (unsigned int)array[i] << " ";
		cout << endl << endl;
	}

	bool image_size_check(const char* filename, DWORD ptLength)
	{
		FILE_INFO info = get_file_info(filename);
		DWORD pixels = info.width * info.height;
		DWORD dataSize = ((ptLength + 16) & ~15) + 8;
		return pixels > dataSize && info.width >= 10 && info.height >= 10;
	}

	BYTE* get_pixels(const char* filename)
	{
		FILE_INFO info = get_file_info(filename);
		FILE* file;
		fopen_s(&file, filename, "rb");
		BYTE* pixels = new BYTE[info.size_padded];
		fseek(file, 54, 0);
		fread(pixels, sizeof(BYTE), info.size_padded, file);
		fclose(file);
		return pixels;
	}

	int get_str_len(const char* str)
	{
		int len = 0;
		while (str[len] != '\0') { len++; }
		return len;
	}

	int get_str_len(const unsigned char* str)
	{
		int len = 0;
		while (str[len] != '\0') { len++; }
		return len;
	}

	void set_pixels(const char* orgFilename, const char* newFilename, BYTE* data, DWORD dataLength)
	{
		copy_header(orgFilename, newFilename, 54);
		FILE_INFO info = get_file_info(orgFilename);
		BYTE* pixels = get_pixels(orgFilename);
		BYTE* newPixels = new BYTE[info.size_padded];
		memset(newPixels, 0, info.size_padded);
		unsigned int dataIndex = 0;

		for (unsigned int i = 0; i < info.height; i++)
		{
			DWORD rowStart = i * info.row_size_padded;
			for (unsigned int j = 0; j < info.row_size; j += 3)
			{
				if (dataIndex < dataLength)
				{
					BIT* bBinary = get_binary(pixels[j + rowStart]);
					BIT* gBinary = get_binary(pixels[j + 1 + rowStart]);
					BIT* rBinary = get_binary(pixels[j + 2 + rowStart]);
					BIT* dBinary = get_binary(data[dataIndex]);
					bBinary[5] = dBinary[0];
					bBinary[6] = dBinary[1];
					bBinary[7] = dBinary[2];
					gBinary[5] = dBinary[3];
					gBinary[6] = dBinary[4];
					gBinary[7] = dBinary[5];
					rBinary[5] = dBinary[6];
					rBinary[6] = dBinary[7];
					BYTE bByte = get_byte(bBinary);
					BYTE gByte = get_byte(gBinary);
					BYTE rByte = get_byte(rBinary);
					newPixels[j + rowStart] = bByte;
					newPixels[j + 1 + rowStart] = gByte;
					newPixels[j + 2 + rowStart] = rByte;
					delete[] bBinary;
					delete[] gBinary;
					delete[] rBinary;
					delete[] dBinary;
				}
				else
				{
					newPixels[j + rowStart] = pixels[j + rowStart];
					newPixels[j + 1 + rowStart] = pixels[j + 1 + rowStart];
					newPixels[j + 2 + rowStart] = pixels[j + 2 + rowStart];
				}

				dataIndex++;
			}
		}

		FILE* newFile;
		fopen_s(&newFile, newFilename, "ab");
		fwrite(newPixels, sizeof(BYTE), info.size_padded, newFile);
		fclose(newFile);
		delete[] newPixels;
		delete[] pixels;
	}

	SRESULT stega_encrypt(const char* orgFilename, const char* newFilename, const BYTE* key, const BYTE* plainText, DWORD ptLength)
	{
		// Ensure image can hold encrypted data
		if (!image_size_check(orgFilename, ptLength)) { return SRESULT_IMAGE_TOO_SMALL;	}

		// Encrypt plain text, saving the cipher text and cipher text size
		DWORD* cipherLength = new DWORD;
		BYTE* cipherData;
		if (aes_encrypt(key, plainText, ptLength, &cipherData, cipherLength)) { return SRESULT_FAILURE; }

		// Create an array to hold the plain text size and cipher text size
		// (both stored using 4-byte int) as well as the cipher text itself
		DWORD dataLength = 8 + *cipherLength;
		BYTE* data = new BYTE[dataLength];

		// Store the size (in bytes) of the plain text
		BYTE* ptLengthArray = get_bytes(ptLength);
		for (int i = 0; i < 4; i++)
			data[i] = ptLengthArray[i];

		// Store the size (in bytes) of the cipher text
		BYTE* cLengthArray = get_bytes(*cipherLength);
		for (int i = 0; i < 4; i++)
			data[4 + i] = cLengthArray[i];

		// Store the cipher text
		for (unsigned int i = 0; i < *cipherLength; i++)
			data[8 + i] = cipherData[i];

#ifdef DEBUG
		print_file_info(get_file_info(orgFilename));
		print_byte_array("Plain text bytes", plainText, ptLength);
		print_byte_array("Cipher data bytes", cipherData, *cipherLength);
		print_byte_array("Bitmap data bytes", data, dataLength);
#endif

		// Save the data by embedding it within the pixel data
		set_pixels(orgFilename, newFilename, data, dataLength);

		// Clean-up
		delete cipherLength;
		delete[] cipherData;
		delete[] data;
		delete[] ptLengthArray;
		delete[] cLengthArray;

		return SRESULT_SUCESSS;
	}

	SRESULT stega_decrypt(const char* filename, const BYTE* key, BYTE** plainText)
	{
		// Get header info
		FILE_INFO info = get_file_info(filename);

		// Get pixel data from image
		BYTE* pixelData = get_pixels(filename);

#ifdef DEBUG
		print_file_info(info);
#endif

		// Get size of plain text data
		DWORD ptLength = get_dword_from_pixels(pixelData, 0);

		// Get size of cipher data
		DWORD cipherLength = get_dword_from_pixels(pixelData, 4);

		// Extract cipher data from pixel data
		BYTE* cipherData = new BYTE[cipherLength];
		memset(cipherData, 0, cipherLength);
		unsigned int cipherIndex = 0;
		BIT* cDataBinary = new BIT[8];
		int offset = 24;
		for (unsigned int i = 0; i < info.height; i++)
		{
			if (cipherIndex >= cipherLength) break;
			DWORD rowStart = i * info.row_size_padded;
			for (unsigned int j = offset; j < info.row_size; j += 3)
			{
				if (cipherIndex >= cipherLength) break;
				BIT* bBinary = get_binary(pixelData[rowStart + j]);
				BIT* gBinary = get_binary(pixelData[rowStart + j + 1]);
				BIT* rBinary = get_binary(pixelData[rowStart + j + 2]);
				cDataBinary[0] = bBinary[5];
				cDataBinary[1] = bBinary[6];
				cDataBinary[2] = bBinary[7];
				cDataBinary[3] = gBinary[5];
				cDataBinary[4] = gBinary[6];
				cDataBinary[5] = gBinary[7];
				cDataBinary[6] = rBinary[5];
				cDataBinary[7] = rBinary[6];
				cipherData[cipherIndex] = get_byte(cDataBinary);
				cipherIndex++;
				delete[] bBinary;
				delete[] gBinary;
				delete[] rBinary;
			}
			offset = 0;
		}

		// Decryt message
		*plainText = new BYTE[ptLength + 16];
		SRESULT result = (aes_decrypt(key, cipherData, cipherLength, *plainText) == 0) ? SRESULT_SUCESSS : SRESULT_FAILURE;

		// Null-terminate the plain text byte array (to create C-style string)
		(*plainText)[ptLength] = '\0';

#ifdef DEBUG
		print_byte_array("Plain text bytes", *plainText, ptLength);
		print_byte_array("Cipher data bytes", cipherData, cipherLength);
#endif

		// Clean-up
		memset(cipherData, 0, cipherLength);
		delete[] pixelData;
		delete[] cDataBinary;
		delete[] cipherData;

		return result;
	}
}