#define _CRT_SECURE_NO_WARNINGS // Agar aman dari error C4996 di Visual Studio
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std;

// Fungsi untuk mengubah objek waktu (time_point) menjadi teks string yang rapi
string formatTanggal(chrono::system_clock::time_point waktu) {
    time_t waktu_c = chrono::system_clock::to_time_t(waktu);
    tm* waktu_lokal = localtime(&waktu_c);

    char buffer[50];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M", waktu_lokal); // Format: Tanggal-Bulan-Tahun Jam:Menit
    return string(buffer);
}

void hitungMasaSewa(int durasiHari) {
    // 1. Ambil waktu saat ini (Otomatis Tanggal Sewa)
    auto waktuSewa = chrono::system_clock::now();

    // 2. Otomatis Hitung Tanggal Kembali (Waktu Sekarang + Jumlah Hari dalam satuan Jam)
    // 1 hari = 24 jam, jadi jika durasi 3 hari = 3 * 24 = 72 jam
    auto waktuKembali = waktuSewa + chrono::hours(24 * durasiHari);

    // 3. Tampilkan Hasilnya
    cout << "=========================================" << endl;
    cout << "          SISTEM DURASI OTOMATIS         " << endl;
    cout << "=========================================" << endl;
    cout << "Durasi Sewa     : " << durasiHari << " Hari" << endl;
    cout << "Tanggal Sewa    : " << formatTanggal(waktuSewa) << endl;
    cout << "Tanggal Kembali : " << formatTanggal(waktuKembali) << endl;
    cout << "=========================================" << endl;
}

int main() {
    int inputHari;
    cout << "Masukkan durasi pinjam barang (hari): ";
    cin >> inputHari;

    hitungMasaSewa(inputHari);

    return 0;
}