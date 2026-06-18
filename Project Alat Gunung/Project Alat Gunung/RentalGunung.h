#ifndef RENTAL_GUNUNG_H
#define RENTAL_GUNUNG_H

#define _CRT_SECURE_NO_WARNINGS // Menghindari warning localtime di Visual Studio
#include <string>
#include <vector>

// Menggunakan namespace std agar tidak perlu menulis std:: lagi
using namespace std;

// ==========================================
// CLASS 1: BARANG
// ==========================================
class Barang {
public:
    string kodeBarang;
    string namaBarang;
    string merk;
    string spesifikasi;
    double hargaPerHari = 0.0; // FIX: Inisialisasi default
    int stok = 0;              // FIX: Inisialisasi default

    Barang();
    Barang(string kb, string nb, string m, string spek, double hrg, int stk);
    void updateStok(int jumlah);
    void ubahDetail(string nama, string m, double harga, string k, string s);
};

// ==========================================
// CLASS 2: TRANSAKSI
// ==========================================
class Transaksi {
public:
    string kodePemesan;
    string namaPemesan;
    string noTelp;
    string jaminan;

    vector<Barang> listBarang;
    vector<int> listJumlahSewa;
    vector<int> listDurasiHari;

    string tanggalPinjam;
    string tanggalKembali;

    double totalHarga = 0.0;   // FIX: Inisialisasi default
    string metodeBayar;
    double uangBayar = 0.0;     // FIX: Inisialisasi default
    double kembalian = 0.0;     // FIX: Inisialisasi default

    void hitungWaktuOtomatis();
    void hitungTotalBiaya();
    void tampilkanRincian();
    void cetakNotaTxt();
   
};

// ==========================================
// CLASS 3: ADMIN
// ==========================================
class Admin {
private:
    string passwordMaster = "adminGunung123";

public:
    bool login();
    void lihatStok(const vector<Barang>& daftarBarang);
    void restokBarang(vector<Barang>& daftarBarang);
    void tambahJenisBarang(vector<Barang>& daftarBarang);
    void monitoringPenyewa(vector<Transaksi>& daftarAktif, vector<Transaksi>& daftarSelesai, vector<Barang>& daftarBarang);
    void monitoringLogSewa(const vector<Transaksi>& daftarSelesai);
   
   
};

// ==========================================
// CLASS 4: SISTEM SEWA (Engine Utama)
// ==========================================
class SistemSewa {
private:
    vector<Barang> databaseBarang;
    vector<Transaksi> transaksiAktif;
    vector<Transaksi> logSelesai;
    Admin adminSistem;

    string generateKode(string prefix);

public:
    void inisialisasiDataAwal();
    void lihatKatalog();
    void fiturSewaBarang();
    void menuAdmin();
    void menuUtama();
    void ubahdata();
};

#endif