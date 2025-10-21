<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type");

require("koneksi.php");
$perintah = "SELECT id, tinggi_air,status, lokasi_ews,
    DATE_FORMAT(waktu, '%W, %d/%m/%Y Jam %H:%i:%s') AS waktu
FROM (
    SELECT * FROM tinggiAir
    ORDER BY id DESC
    LIMIT 30
) AS sub
ORDER BY id ASC";
$eksekusi = mysqli_query($konek, $perintah);
$cek = mysqli_affected_rows($konek);

if ($cek > 0) {
    $response["kode"] = 1;
    $response["pesan"] = "DataTersedia";
    $response["data"] = array();

    while ($ambil = mysqli_fetch_object($eksekusi)) {
        $F["id"] = $ambil->id;
        $F["tinggi_air"] =  $ambil->tinggi_air;
        $F["lokasi_ews"] =  $ambil->lokasi_ews;
        $F["waktu"] = $ambil->waktu;
        $F["status"] = $ambil->status;



        array_push($response["data"], $F);
    }
} else {
    $response["kode"] = 0;
    $response["pesan"] = "Data Not Found";
}
echo json_encode($response);
mysqli_close($konek);
