/****************************************************************************
**					SAKARYA ÜNİVERSİTESİ
**			         BİLGİSAYAR VE BİLİŞİM BİLİMLERİ FAKÜLTESİ
**				    BİLGİSAYAR MÜHENDİSLİĞİ BÖLÜMÜ
**				          PROGRAMLAMAYA GİRİŞİ DERSİ
**
**				ÖDEV NUMARASI...:	HALİL
**				ÖĞRENCİ ADI...............:	AKYILDIZ
**				ÖĞRENCİ NUMARASI.:	G171210043
**				DERS GRUBU:	2-A
****************************************************************************/

#include <iostream>
#include <Windows.h>

using namespace std;


struct UcakMermisi
{
	int x;
	int y;
	char karktr = 219;
	bool vurdumu;
};
struct Dusman
{
	int x;
	int y;
	bool vuruldumu = false;
	char kar = 177;
};
enum YON
{
	YON_YUKARI = 3,
	YON_ASAGI = 4
};
struct UCAK
{
	int x;
	int y;
	int ucakGenislik = 3;
	int ucakYukseklik = 5;

	YON yon;
	char karakter;
};

const int genislik = 80;
const int yukseklik = 20;
bool hareketAktifmi;
bool atesAktifmi;
bool dusmanAktifmi;
int dusmanSayac = 0;

char sahne[genislik][yukseklik];
UcakMermisi mermiler[1000];
Dusman dusmanlar[1000];
char tuslar[256];

UCAK ucak;
int mermiSirasi = 0;
int dusmanSirasi = 0;

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void kursoruGizle()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void UcakOlustur() //Uçağı oluşturup ilk kordinatlarını ve yönünü atar
{
	ucak.x = 2;
	ucak.y = 0;
	ucak.yon = YON_ASAGI;
	ucak.karakter = 219;

}
void ucakSahneyeYerlestir()	// Uçağı ekrana basar
{
	int x = ucak.x;
	int y = ucak.y;

	sahne[1][y + 1] = 219;
	sahne[2][y + 2] = 219;
	sahne[3][y + 3] = 219;
	sahne[2][y + 4] = 219;
	sahne[1][y + 5] = 219;
}
void UcakHareketEttir() // Uçağı yöne göre hareket ettirir
{
	if (ucak.yon == YON_YUKARI)
	{
		if (ucak.y != 0)	// Hareket sahne sınırlarını aşamaz
			ucak.y--;

	}
	if (ucak.yon == YON_ASAGI)
	{
		if (ucak.y != yukseklik - 7)
			ucak.y++;

	}
}
void klavyeOku(char tuslar[])
{
	for (int x = 0; x < 256; x++)
	{
		tuslar[x] = (char)(GetAsyncKeyState(x) >> 8);
	}
}

void klavyeKontrol()
{
	klavyeOku(tuslar);
	if (tuslar['W'] != 0)
	{
		ucak.yon = YON_YUKARI;
		hareketAktifmi = true;

	}
	if (tuslar['S'] != 0)
	{
		ucak.yon = YON_ASAGI;
		hareketAktifmi = true;
	}
	if (tuslar['P'] != 0)
	{
		atesAktifmi = true;
	}
}
void DusmanCikar() // Düşmanı belirli aralıklarla çıkarmak için sayaç methodu.Yaklaşık 2 saniyede bir düşman çıkartır
{
	dusmanSayac += 50;
	if (dusmanSayac >= 2000)
	{
		dusmanAktifmi = true;
		dusmanSayac = 0;
	}
}
void DusmanOlustur() // Düşmanın ilk kordinatlarını atar.
{
	dusmanlar[dusmanSirasi].x = genislik;
	dusmanlar[dusmanSirasi].y = rand() % 16 + 1; // Sahnenin herhangi bir yerinden gelmesi için y kordinatına random atanır

	dusmanSirasi++;
}
void DusmanHareketEttir()	//Düşmanı sürekli sola hareket ettirir
{
	for (int i = 0; i < dusmanSirasi; i++)
	{
		if (dusmanlar[i].x != 0 && dusmanlar[i].vuruldumu == false)
			dusmanlar[i].x--;
	}
}
void DusmanıSahneyeYerlestir()	// Düşmanı Düşman sayısı kadar döner ve ekrana basar
{
	for (int i = 0; i < dusmanSirasi; i++)
	{
		if (dusmanlar[i].x != 0) // Sahnenin sonuna geldiyse erkana basmaz
		{
			for (int j = 0; j < mermiSirasi; j++)	// Düşmanın vurulup vurulmadığını bulur
			{
				if (mermiler[j].x == dusmanlar[i].x && mermiler[j].y == dusmanlar[i].y) // Mermi ile düşmanın kordinatlarını karşılaştırır
				{
					dusmanlar[i].vuruldumu = true;
					dusmanlar[i].x = genislik + 1;
					mermiler[j].vurdumu = true;
					mermiler[j].x = 0;
				}
				else if (mermiler[j].x == dusmanlar[i].x && mermiler[j].y == dusmanlar[i].y + 1)
				{
					dusmanlar[i].vuruldumu = true;
					mermiler[j].vurdumu = true;
					mermiler[j].x = 0;
				}
				else if (mermiler[j].x == dusmanlar[i].x && mermiler[j].y == dusmanlar[i].y + 2)
				{
					dusmanlar[i].vuruldumu = true;
					mermiler[j].vurdumu = true;
					mermiler[j].x = 0;
				}
			}

			if (dusmanlar[i].vuruldumu == false) // Düşmanı hücre hücre ekrana basar
			{
				sahne[dusmanlar[i].x][dusmanlar[i].y] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x + 1][dusmanlar[i].y] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x + 2][dusmanlar[i].y] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x][dusmanlar[i].y + 1] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x + 1][dusmanlar[i].y + 1] = ' ';
				sahne[dusmanlar[i].x + 2][dusmanlar[i].y + 1] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x][dusmanlar[i].y + 2] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x + 1][dusmanlar[i].y + 2] = dusmanlar[i].kar;
				sahne[dusmanlar[i].x + 2][dusmanlar[i].y + 2] = dusmanlar[i].kar;
			}

		}

	}
}
void UcakAtesEttir() // Merminin uçaktan çıkacağı yere göre kordinatlarını atar
{
	mermiler[mermiSirasi].x = 4;
	mermiler[mermiSirasi].y = ucak.y + 3;

	mermiSirasi++;
}
void MermiHareketEttir() // Mermileri sirasıyla sağa hareket ettirir
{
	for (int i = 0; i < mermiSirasi; i++)
	{
		if (mermiler[i].x != genislik - 1 && mermiler[i].vurdumu == false)
			mermiler[i].x++;
	}
}
void MermiyiSahneyeYerlestir() // Mermileri mermi sayısı kadar dönerek ekrana basar
{
	for (int i = 0; i < mermiSirasi; i++)
	{
		if (mermiler[i].vurdumu == false)
			sahne[mermiler[i].x][mermiler[i].y] = mermiler[i].karktr;
	}

}
void sahneyiCiz() // Sahneyi ekrana basar
{
	for (int y = 0; y < yukseklik; y++)
	{
		for (int x = 0; x < genislik; x++)
		{
			cout << sahne[x][y];
		}
		cout << endl;
	}
}
void sahneyiTemizle() // Sahneyi siler
{
	for (int y = 0; y < yukseklik; y++)
	{
		for (int x = 0; x < genislik; x++)
		{
			sahne[x][y] = ' ';
		}
	}
}
void sinirlariOlustur() // Sahne sınırlarını oluşturur
{
	for (int x = 0; x < genislik; x++)
	{
		sahne[x][0] = 219;
		sahne[x][yukseklik - 1] = 219;
	}
	for (int y = 0; y < yukseklik; y++)
	{
		sahne[0][y] = 219;
		sahne[genislik - 1][y] = 219;
	}
}

int main()
{
	kursoruGizle();
	klavyeOku(tuslar);

	UcakOlustur();

	while (true)
	{
		sahneyiTemizle();
		sinirlariOlustur();
		klavyeKontrol();
		ucakSahneyeYerlestir();

		if (hareketAktifmi)	// Uçağın tuşa basıldığında hareket etmesi için konulan sorgu
		{
			UcakHareketEttir();
			hareketAktifmi = false;
		}
		DusmanCikar();
		if (dusmanAktifmi)	// Belirli aralıklarla düşman çıkarılması için konulan sorgu
		{
			DusmanOlustur();
			dusmanAktifmi = false;

		}
		DusmanHareketEttir();
		DusmanıSahneyeYerlestir();
		if (atesAktifmi)	// Tuşa basıldığında mermi fırlatılması için konulan sorgu
		{
			UcakAtesEttir();
			atesAktifmi = false;
		}
		MermiHareketEttir();
		MermiyiSahneyeYerlestir();



		gotoxy(0, 0);
		sahneyiCiz();
		Sleep(20);
	}

	cin.get();
}