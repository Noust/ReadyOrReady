#include "include.h"

void FManager::ReadSettings() {
    std::ifstream file("sarilla.bin", std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        // Get file size
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Check if file size matches settings size
        if (fileSize != sizeof(RonSettings::settings)) {
            file.close();
            // Delete invalid file and create new one
            std::remove("sarilla.bin");
            SaveSettings();
            return;
        }

        // Read settings if size matches
        file.read(reinterpret_cast<char*>(&RonSettings::settings), sizeof(RonSettings::settings));
        file.close();
    }
}

// Funcin para guardar la configuracin en un archivo binario
void FManager::SaveSettings() {
    std::ofstream file("sarilla.bin", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&RonSettings::settings), sizeof(RonSettings::settings));
        file.close();
    }
}