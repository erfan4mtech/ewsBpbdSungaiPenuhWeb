plugins {
    id("com.android.application")
}

android {
    namespace = "com.solusidigitalandalas.tvewsbanjir"
    compileSdk = 34  // stabil, gak perlu 36

    defaultConfig {
        applicationId = "com.solusidigitalandalas.tvewsbanjir"
        minSdk = 21
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    buildFeatures {
        compose = false   // ❌ matikan Compose
        viewBinding = true // ✅ aktifkan ViewBinding (biar gampang akses UI)
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.13.1")
    implementation("androidx.appcompat:appcompat:1.7.0")
    implementation("com.google.android.material:material:1.12.0")
    implementation("androidx.constraintlayout:constraintlayout:2.1.4")
}
