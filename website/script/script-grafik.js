

// const apiURL = "https://parkir.udnp4mtechno.com/api/getTinggiAir.php"; 

// const ctx = document.getElementById("chartTinggiAir").getContext("2d");
// let chart;
// const statusEl = document.getElementById("status");
// const tbody = document.querySelector("#dataTable tbody");
// const tahun = document.getElementById("tahun");
// tahun.textContent = new Date().getFullYear();

// async function ambilData() {
//   statusEl.textContent = "Mengambil data...";
//   try {
//     const res = await fetch(apiURL);
//     if (!res.ok) throw new Error("HTTP " + res.status);
//     const json = await res.json();

//     if (json.kode === 1) {
//       statusEl.textContent = "Data berhasil dimuat.";
//       tampilkanData(json.data);
//       tampilkanGrafik(json.data);
//     } else {
//       statusEl.textContent = "Tidak ada data.";
//     }
//   } catch (err) {
//     statusEl.textContent = "Gagal memuat data: " + err.message;
//   }
// }

// function tampilkanData(data) {
//   tbody.innerHTML = "";
//   data.forEach(d => {
//     const tr = document.createElement("tr");
//     tr.innerHTML = `
//       <td>${d.id}</td>
//       <td>${d.tinggi_air}</td>
//       <td>${d.waktu}</td>
//     `;
//     tbody.appendChild(tr);
//   });
// }

// function tampilkanGrafik(data) {
//   const labels = data.map(d => d.waktu);
//   const tinggiAir = data.map(d => parseFloat(d.tinggi_air));

//   if (chart) chart.destroy(); // hapus grafik lama

//   chart = new Chart(ctx, {
//     type: "line",
//     data: {
//       labels,
//       datasets: [{
//         label: "Tinggi Air (Meter)",
//         data: tinggiAir,
//         borderColor: "#2b7cff",
//         backgroundColor: "rgba(43,124,255,0.15)",
//         borderWidth: 2,
//         pointRadius: 3,
//         fill: true,
//         tension: 0.3
//       }]
//     },
//     options: {
//       responsive: true,
//       maintainAspectRatio: false,
//       plugins: {
//         legend: { position: "top" },
//         title: { display: true, text: "Grafik Tinggi Air Terbaru" }
//       },
//       scales: {
//         x: { title: { display: true, text: "Waktu" } },
//         y: { title: { display: true, text: "Tinggi Air (Meter)" } }
//       }
//     }
//   });
// }

// // tombol manual refresh
// document.getElementById("btnRefresh").addEventListener("click", ambilData);

// // auto-refresh
// const intervalSelect = document.getElementById("refreshInterval");
// let intervalID;

// intervalSelect.addEventListener("change", () => {
//   clearInterval(intervalID);
//   const val = parseInt(intervalSelect.value);
//   if (val > 0) intervalID = setInterval(ambilData, val);
// });

// // pertama kali
// ambilData();


const apiURL = "https://parkir.udnp4mtechno.com/api/getTinggiAir.php"; 

const ctx = document.getElementById("chartTinggiAir").getContext("2d");
let chart;

// elemen-elemen utama
const tbody = document.querySelector("#dataTable tbody");

// Fungsi utama untuk ambil data dari API
async function ambilData() {
  try {
    const res = await fetch(apiURL);
    if (!res.ok) throw new Error("HTTP " + res.status);
    const json = await res.json();

    if (json.kode === 1) {
      tampilkanData(json.data);
      tampilkanGrafik(json.data);
      console.log("Data berhasil dimuat.");
    } else {
      console.warn("Tidak ada data dari API.");
    }
  } catch (err) {
    console.error("Gagal memuat data:", err.message);
  }
}

// Fungsi untuk menampilkan data ke tabel
function tampilkanData(data) {
  if (!tbody) return;
  tbody.innerHTML = "";
  data.forEach(d => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>${d.id}</td>
      <td>${d.tinggi_air}</td>
      <td>${d.lokasi_ews || '-'}</td>
      <td>${d.waktu}</td>
    `;
    tbody.appendChild(tr);
  });
}

// Fungsi untuk menampilkan grafik
function tampilkanGrafik(data) {
  const labels = data.map(d => d.waktu);
  const tinggiAir = data.map(d => parseFloat(d.tinggi_air));

  if (chart) chart.destroy(); // hapus grafik lama jika ada

  chart = new Chart(ctx, {
    type: "line",
    data: {
      labels,
      datasets: [{
        label: "Tinggi Air (Meter)",
        data: tinggiAir,
        borderColor: "#2b7cff",
        backgroundColor: "rgba(43,124,255,0.15)",
        borderWidth: 2,
        pointRadius: 3,
        fill: true,
        tension: 0.3
      }]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: {
        legend: { position: "top" },
        title: { display: true, text: "Grafik Tinggi Air Terbaru" }
      },
      scales: {
        x: { title: { display: true, text: "Waktu" } },
        y: { title: { display: true, text: "Tinggi Air (Meter)" } }
      }
    }
  });
}

// tombol manual refresh
const btnRefresh = document.getElementById("btnRefresh");
if (btnRefresh) {
  btnRefresh.addEventListener("click", ambilData);
}

// auto-refresh
const intervalSelect = document.getElementById("refreshInterval");
let intervalID;

if (intervalSelect) {
  intervalSelect.addEventListener("change", () => {
    clearInterval(intervalID);
    const val = parseInt(intervalSelect.value);
    if (val > 0) intervalID = setInterval(ambilData, val);
  });
}

// pertama kali ambil data
ambilData();
