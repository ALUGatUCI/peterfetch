#include <iostream>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <args.hxx>

#include <config.hpp>

int main(int argc, char *argv[]) {
    args::ArgumentParser parser("peterfetch v" PROJECT_VERSION "\n" PROJECT_DESCRIPTION);
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::CompletionFlag completion(parser, {"complete"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Completion &e) {
        std::cerr << e.what();
        return 1;
    } catch (const args::Help &e) {
        std::cerr << parser;
        return 0;
    } catch (const args::ParseError &e) {
        std::cerr << e.what() << "\n\n";
        std::cerr << parser;
        return 1;
    }

    try {
        curlpp::Easy request;
        request.setOpt<curlpp::options::Url>(ANTEATERAPI_URL);

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
