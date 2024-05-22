#pragma once

namespace raytracing {

class Resources {
   public:
    static std::string GetFonts() {
        // TODO: Is there a better way to find the resources so that they don't have to be relative?
        return "./resources/font/";
    }
};

} // namespace raytracing