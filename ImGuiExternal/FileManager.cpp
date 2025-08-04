#include "include.h"

void FManager::ReadSettings() {
    std::ifstream file("sarilla.bin", std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        // Get file size
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Check if file size matches settings size
        if (fileSize != sizeof(CSettings::settings)) {
            file.close();
            // Delete invalid file and create new one
            std::remove("sarilla.bin");
            SaveSettings();
            return;
        }

        // Read settings if size matches
        file.read(reinterpret_cast<char*>(&CSettings::settings), sizeof(CSettings::settings));
        file.close();
    }
}

// Funcin para guardar la configuracin en un archivo binario
void FManager::SaveSettings() {
    std::ofstream file("sarilla.bin", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&CSettings::settings), sizeof(CSettings::settings));
        file.close();
    }
}