/**
* @file test.ixx
* @author Andrew Fagan
* @date 2/7/2025
*/
export module Astral.Test;

export import :test2;

namespace Core {

    class ABC
    {
    public:
        ABC();

        void a();

        void b();

        int c();

    private:
        int abc;
    };
}
