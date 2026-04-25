#include <iostream>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

int main() {
    try {
        curlpp::Easy request;
        request.setOpt<curlpp::options::Url>("https://anteaterapi.com");

        std::stringstream response;
        curlpp::options::WriteStream ws(&response);
        request.setOpt(ws);
        request.perform();

        std::cout << "Got\n\n";
        std::cout << response.str();
    } catch(curlpp::RuntimeError & e) {
        std::cout << e.what() << std::endl;
    } catch(curlpp::LogicError & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
