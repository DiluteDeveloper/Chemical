#pragma once


namespace Chemical {

    namespace Util {
        class Stopwatch
        {
            using clock = std::chrono::steady_clock;
            std::chrono::time_point<clock> start;

        public:
            Stopwatch()
                : start(clock::now()) {}

            std::chrono::duration<double> Elapsed() const
            {
                return std::chrono::duration<double>(clock::now() - start);
            }

            void Reset()
            {
                start = clock::now();
            }
        };
    }


}

