<?php
header("Content-Type: application/json");

$host = "localhost";
$user = "";
$pass = "";
$db = "";

$konek = mysqli_connect($host, $user, $pass, $db) or die("Database Tidak Terhubung");



mysqli_query($konek, "SET time_zone = '+07:00'");
