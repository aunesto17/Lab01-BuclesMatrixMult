#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Function to get the system's page size dynamically
long getPageSize() {
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
#else
    return sysconf(_SC_PAGESIZE);
#endif
}

int main() {
    long pageSizeBytes = getPageSize();
    // A standard int is usually 4 bytes. 
    // If page size is 4096 bytes, elementsPerPage is 1024.
    long elementsPerPage = pageSizeBytes / sizeof(int); 

    // We will test increasing number of pages to see exactly where the TLB fills up.
    // Starting at 16 pages up to 131072 pages (512 MB)
    const long MIN_PAGES = 16;
    const long MAX_PAGES = 131072; 

    std::ofstream csvFile("tlb_results.csv");
    csvFile << "PagesAccessed,TotalSize_MB,TLBFriendly_ns,TLBUnfriendly_ns\n";

    std::cout << "--- Iniciando Benchmark de Translation-Lookaside Buffer (TLB) ---\n";
    std::cout << "Tamaño de página detectado: " << pageSizeBytes << " bytes\n\n";

    for (long numPages = MIN_PAGES; numPages <= MAX_PAGES; numPages *= 2) {
        
        long totalElements = numPages * elementsPerPage;
        double sizeMB = (totalElements * sizeof(int)) / (1024.0 * 1024.0);
        
        std::cout << "Probando " << numPages << " páginas (" 
                  << std::fixed << std::setprecision(2) << sizeMB << " MB)..." << std::flush;

        std::vector<int> data(totalElements, 1);
        volatile int sum = 0;

        // --- Test 1: TLB-Friendly (Sequential) ---
        // Reads consecutive memory. It only causes a TLB miss once every 1024 elements.
        auto start_friendly = std::chrono::high_resolution_clock::now();
        for (long i = 0; i < totalElements; ++i) {
            sum += data[i];
        }
        auto end_friendly = std::chrono::high_resolution_clock::now();
        double friendly_ns = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end_friendly - start_friendly).count()) / totalElements;


        sum = 0; // Reset sum

        // --- Test 2: TLB-Unfriendly (Page Striding) ---
        // We do the exact same number of reads, but we jump by exactly 'elementsPerPage'
        // This forces a new page access on EVERY single read.
        auto start_unfriendly = std::chrono::high_resolution_clock::now();
        
        long currentIndex = 0;
        for (long i = 0; i < totalElements; ++i) {
            sum += data[currentIndex];
            
            // Jump by one page. If we hit the end of the array, wrap around to the next offset.
            currentIndex += elementsPerPage;
            if (currentIndex >= totalElements) {
                // Wrap around to a new element within the first page
                currentIndex = (currentIndex % totalElements) + 1; 
            }
        }
        auto end_unfriendly = std::chrono::high_resolution_clock::now();
        double unfriendly_ns = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end_unfriendly - start_unfriendly).count()) / totalElements;

        std::cout << " Listo. Friendly: " << friendly_ns << " ns/op | Unfriendly (TLB Thrash): " << unfriendly_ns << " ns/op\n";

        // Guardar en CSV
        csvFile << numPages << "," << sizeMB << "," << friendly_ns << "," << unfriendly_ns << "\n";
    }

    csvFile.close();
    std::cout << "\nResultados guardados en 'tlb_results.csv'." << std::endl;
    return 0;
}