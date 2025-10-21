<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

require("koneksi.php");

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $tinggi_air = $_POST["tinggi_air"];
    $lokasi_ews = $_POST["lokasi_ews"];
    $status = $_POST["status"];


    // 1. Siapkan statement
    $perintah = "INSERT INTO tinggiAir (tinggi_air, lokasi_ews, status) VALUES (?,?,?)";
    $stmt = mysqli_prepare($konek, $perintah);

    // 2. Cek apakah prepare berhasil
    if ($stmt) {
        // 3. Bind parameter
        mysqli_stmt_bind_param($stmt, "dss", $tinggi_air, $lokasi_ews, $status);

        // 4. Eksekusi
        if (mysqli_stmt_execute($stmt)) {
            $response["kode"] = 1;
            $response["pesan"] = "Data Berhasil Disimpan";
        } else {
            $response["kode"] = 0;
            $response["pesan"] = "Gagal Menyimpan Data: " . mysqli_error($konek);
        }

        mysqli_stmt_close($stmt);
    } else {
        $response["kode"] = 0;
        $response["pesan"] = "Prepare statement gagal: " . mysqli_error($konek);
    }
} else {
    $response["kode"] = 0;
    $response["pesan"] = "Metode tidak diizinkan";
}

echo json_encode($response);
mysqli_close($konek);
