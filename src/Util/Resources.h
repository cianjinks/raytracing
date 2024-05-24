#pragma once

namespace raytracing {

class Resources {
   public:
    static std::string GetFonts() {
        // TODO: Is there a better way to find the resources so that they don't have to be relative?
        return "./resources/font/";
    }

    static std::string GetImages() {
        return "./resources/img/";
    }
};

} // namespace raytracing