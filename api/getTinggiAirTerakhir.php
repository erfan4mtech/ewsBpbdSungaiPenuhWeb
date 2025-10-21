<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type");
require("koneksi.php");
$perintah = "SELECT 
    id, 
    tinggi_air, 
    status,
    lokasi_ews,
    DATE_FORMAT(waktu, '%W, %d/%m/%Y Jam %H:%i:%s') AS waktu
FROM tinggiAir
ORDER BY id DESC
LIMIT 1";
$eksekusi = mysqli_query($konek, $perintah);
$cek = mysqli_affected_rows($konek);

if ($cek > 0) {
    $respon["kode"] = 1;
    $respon["pesan"] = "Data Tersedia";
    $respon["data"] = array();

    while ($ambil = mysqli_fetch_object($eksekusi)) {
        $F["id"] = $ambil->id;
        $F["tinggi_air"] = $ambil->tinggi_air;
        $F["status"] = $ambil->status;
        $F["waktu"] = $ambil->waktu;
        $F["lokasi_ews"] = $ambil->lokasi_ews;
        array_push($respon["data"], $F);
    }
} else {
    $respon["kode"] = 0;
    $respon["pesan"] = "Data Not Found";
}
echo json_encode($respon);
mysqli_close($konek);
