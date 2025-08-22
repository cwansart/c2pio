#include <iostream>
#include <sstream>

#include <cstdint>
#include <cstring>
#include <cerrno>

#include <curl/curl.h>

#include "version.h"

// TODO: https://ec.haxx.se/libcurl/performance.html
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Missing arguments!\n";
        // TODO: make out-file optional, use file name in url instead
        std::cout << "Usage: " << argv[0] << " <url> <out-file>\n\n"
                  << "  <url>        url of file to download\n\n"
                  << "  <out-file>   path to save the file to\n";
        return 1;
    }

    std::string url{argv[1]};
    std::string outFile{argv[2]};

    CURL* curl = curl_easy_init();
    char errbuf[CURL_ERROR_SIZE];
    errbuf[0] = 0;
    CURLcode ret = CURLE_OK;

    bool isOK = true;
    FILE* outFileFp = NULL;

    curl_version_info_data *curlVersionInfo = curl_version_info(CURLVERSION_NOW);

    std::stringstream agent;
    agent << argv[0]  << '/'
          << VERSION_MAJOR << '.'
          << VERSION_MINOR << '.'
          << VERSION_PATCH; 
    std::cout << "user agent: " << agent.str() << '\n';

    std::cout << "using curl version: " << curlVersionInfo->version << '\n'
              << "attempt to download " << url << '\n';

    // curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, CURL_MAX_READ_SIZE);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // CURLOPT_DEBUGFUNCTION, see: https://ec.haxx.se/libcurl/verbose.html

    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.str().c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);

    
    outFileFp = std::fopen(outFile.c_str(), "wb");
    if (outFileFp == NULL) {
        std::cerr << "failed to open file: " << std::strerror(errno) << '\n';
        isOK = false;
        goto cleanup;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outFileFp);

    ret = curl_easy_perform(curl);

    isOK = ret == CURLE_OK;
    if (isOK) {
        std::cout << "written file to " << outFile << '\n';
    } else {
        if (errbuf[0] != 0) {
            std::cerr << "error: " << errbuf << '\n';
        } else {
            std::cerr << "error: " << curl_easy_strerror(ret) << '\n';
        }
    }

cleanup:
    if (outFileFp != NULL) {
        std::fclose(outFileFp);
        outFileFp = NULL;
    }

    if (curl != NULL) {
        curl_easy_cleanup(curl);
        curl = NULL;
    }

    return isOK ? 0 : 1;
}
