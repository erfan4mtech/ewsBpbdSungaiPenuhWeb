<?php

require("koneksi.php");

$response = array();

if($_SERVER['REQUEST_METHOD'] == 'POST'){

    $uName = $_POST["user_name"];
    $uPass = $_POST["user_pass"];

    // persiapkan query dengan placeholder
    $perintah = "SELECT * FROM admins WHERE user_name = ? AND user_pass = ?";

    //Persiapkan statement
    if($stmt = mysqli_prepare($konek, $perintah)){
        mysqli_stmt_bind_param($stmt,"ss", $uName, $uPass);

        mysqli_stmt_execute($stmt);

        $result = mysqli_stmt_get_result($stmt);

        if(mysqli_num_rows($result) > 0){
            $response["kode"] = 1;
            $response["pesan"] = "Data Di Temukan";
            $response["data"] = array();

            //ambil data dari hasil query
            while($ambil = mysqli_fetch_object($result)){
                 $F["id"] = $ambil -> id;
                 $F["user_nama"] = $ambil -> user_name;
                 $F["user_pass"] = $ambil -> user_pass;
                 $F["nama_lengkap"] = $ambil -> nama_lengkap;
            

                 array_push($response["data"], $F);
            }
           

        }else{
            $response["kode"] = 0;
            $response["pesan"] = "Data Tidak Ditemukan";
        }

    }else{
        $response["kode"] = 0;
        $response["pesan"] = "Gagal Menyiapkan Query";
    }


}else{
    $response["kode"] = 0;
    $response["pesan"] = "Tidak Ada Post Data";

}
echo json_encode($response);
mysqli_close($konek);