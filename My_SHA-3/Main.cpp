#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <chrono>
using namespace std;

unsigned int circularLeftShift(unsigned int value, int shift) 
{
	return (value << shift) | (value >> (32 - shift));
}

string SHA_1(string message)
{
	unsigned int h0 = 0x67452301;
	unsigned int h1 = 0xEFCDAB89;
	unsigned int h2 = 0x98BADCFE;
	unsigned int h3 = 0x10325476;
	unsigned int h4 = 0xC3D2E1F0;

	unsigned long long lengthBeforePadding = message.length() * 8;
	message += (char)0x80;
	while (message.length() % 64 != 56)
	{
		message += (char)0;
	}
	long length = message.length() * 8;

	for (int i = 7; i >= 0; i--)
	{
		message += (char)((lengthBeforePadding >> (8 * i) & 0xFF));
	}

	for (int i = 0; i < ceil(message.length() / (512.0 / 8.0)); i++)
	{
		string tmp = message.substr(i * (512 / 8), (512 / 8));
		vector<unsigned int> M;
		for (int j = 0; j <= 15; j++)
		{
			unsigned int value = 0;
			for (int k = 0; k < 4; k++) {
				if (j * 4 + k < tmp.length())
				{
					value = (value << 8) | (unsigned char)tmp[j * 4 + k];
				}
			}
			M.push_back(value);
		}
		vector<unsigned int> W = M;
		for (int j = 16; j <= 79; j++)
		{
			W.push_back(circularLeftShift(W[j - 3] ^ W[j - 8] ^ W[j - 14] ^ W[j - 16], 1));
		}

		auto a = h0;
		auto b = h1;
		auto c = h2;
		auto d = h3;
		auto e = h4;

		unsigned int f;
		unsigned int k;

		for (int j = 0; j <= 79; j++)
		{
			if (j >= 0 && j <= 19)
			{
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if (j >= 20 && j <= 39)
			{
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (j >= 40 && j <= 59)
			{
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else if (j >= 60 && j <= 79)
			{
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}

			unsigned int temp = circularLeftShift(a, 5) + f + e + k + W[j];
			e = d;
			d = c;
			c = circularLeftShift(b, 30);
			b = a;
			a = temp;
		}

		h0 = h0 + a;
		h1 = h1 + b;
		h2 = h2 + c;
		h3 = h3 + d;
		h4 = h4 + e;
	}

	ostringstream oss;
	oss << hex << setfill('0') << setw(8) << h0 << ' ' << setw(8) << h1 << ' ' << setw(8) << h2 << ' ' << setw(8) << h3 << ' ' << setw(8) << h4;

	string hash = oss.str();

	cout << hash << endl;

	return hash;
}

int main()
{
	string message = "The quick brown fox jumps over the lazy doggggggggggggggggggggjidnfijgnjssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvth"; // 9d72930165b55f9e3cb01f3b8493c8a9e40679b9 - online sha1
	auto start = chrono::high_resolution_clock::now();
	SHA_1(message);
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Time: " << duration.count() << " microseconds" << endl;

	message = "MessageMySha1"; // b4a451d43137596c92e4d913ad2f5745813b9fd9 - online sha1
	start = chrono::high_resolution_clock::now();
	SHA_1(message);
	end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Time: " << duration.count() << " microseconds" << endl;

	// I believe that this is a sufficiently high hashing speed
	return 0;
}