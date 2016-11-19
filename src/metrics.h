// Copyright (c) 2016 The Zcash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uint256.h"

#include <atomic>
#include <string>

struct AtomicCounter {
    std::atomic<uint64_t> value;

    AtomicCounter() : value {0} { }

    void increment(){
        ++value;
    }

    void decrement(){
        --value;
    }

    int get(){
        return value.load();
    }
};

extern AtomicCounter transactionsValidated;
extern AtomicCounter ehSolverRuns;
extern AtomicCounter solutionTargetChecks;

void TrackMinedBlock(uint256 hash);

void ConnectMetricsScreen();
void ThreadShowMetricsScreen();

/**
 * Heart image: https://commons.wikimedia.org/wiki/File:Heart_coraz%C3%B3n.svg
 * License: CC BY-SA 3.0
 *
 * Rendering options:
 * Logo: img2txt -W 90 -H 20 -f utf8 -d none -g design.png >> design.ansi
 */
const std::string METRICS_ART =
"                                       [0;31;40m@[0m                                                                                                              \n"
"                                  [0;1;30;90;43m  [0;31;43m;:::;;;t%%X8[0;33;41m88[0m                                                                                                    \n"
"                               [0;31;43mS[0;1;30;90;43m [0;1;33;93;43m.:...[0;1;31;91;43m888[0;31;43m;;;;[0;1;31;91;43m888[0;31;43m;t%X8[0m                                                                                                 \n"
"                             [0;31;43mS[0;1;33;93;43m......[0;1;31;91;43m88888[0;31;43m;tttttttt%tt%8[0m                                                      [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m                        \n"
"                            [0;31;43mt[0;1;30;90;43m. .[0;31;43mt@[0;33;41m8@@8[0;31;43m@SSSSSXX@XX%%%%%S8[0;33;41m8[0m                                                   [0;1;35;95;45m:[0;35;5;45;105m.....[0;1;35;95;45m;;;;;t[0;35;5;45;105m......[0m                        \n"
"                           [0;1;30;90;43m [0;31;43m;[0;1;30;90;43m.[0;31;43mX[0;33;41m@[0;30;41mXXXXX[0;33;41mX8[0;31;43m8@@8[0;33;41m8X[0;30;41mX[0;31;40m8XX8[0;33;41mX8[0;31;43mSSX@88[0m                                             [0;1;35;95;45m;t[0;35;5;45;105m........................;[0;1;35;95;45m;[0m                   \n"
"                           [0;1;30;90;43m [0;31;43m:S8@X@[0;33;41m8S[0;30;41mS%[0;31;43m8[0;1;31;91;43m888[0;31;43m%[0;33;41m8[0;30;41m%@@[0;33;41m%XS[0;1;30;90;41m@[0;30;41mX@[0;31;43m8X@88[0m                                        [0;34;45m.[0;35;5;45;105m;...................................[0;1;35;95;45m:[0m              \n"
"                           [0;1;30;90;43m [0;31;43m.%X@8[0;33;41m888[0;31;43m@[0;1;31;91;43m88[0;1;33;93;43m.[0;1;31;91;43m88[0;31;43m%X8[0;33;41mX[0;30;41m@8X[0;33;41mX@@X@[0;31;43mSX888[0m                                    [0;35;5;45;105m...................;[0;1;34;94;45m8888[0;1;35;95;45m;[0;35;5;45;105m..................:[0;34;45m;[0m          \n"
"                           [0;1;30;90;43m [0;1;33;93;43m...[0;1;31;91;43m888[0;31;43mSSS%[0;1;31;91;43m8[0;31;43m%XXXS@@XX88[0;33;41m8888[0;31;43mSS@@[0m                                  [0;35;5;45;105m.............[0;1;35;95;45m;[0m  [0;1;35;95;45m;[0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m  [0;1;35;95;45m;[0;35;5;45;105m:...........[0m         \n"
"                         [0;31;43m8:[0;1;33;93;43m.[0;1;31;91;43m88[0;31;43m%@88@8[0;30;41m@[0;31;40m@[0;30;41m8[0;33;41mS[0;30;41mS[0;31;40m8@[0;30;41m@[0;33;41m@88[0;31;43m8X%%%%S%SX@[0m                               [0;1;34;94;45m8[0;35;5;45;105m...........[0;34;45m;[0m       [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m      [0;34;45m:[0;1;35;95;45m;[0;35;5;45;105m.....[0m           \n"
"                        [0;31;43m8[0;33;41m8[0;1;31;91;43m888[0;31;43m@[0;33;41mXXX@[0;30;41mS88[0;31;40m888@XX@[0;30;41mX[0;33;41m@8@X8[0;31;43m8@SS%S8X[0m                              [0;35;5;45;105m..........[0;1;34;94;45m8[0m          [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m          [0;1;35;95;45m;[0m             \n"
"                        [0;31;43mXXt;S[0;33;41m@[0;30;41m@88S[0;33;41mX@8@S%ttt[0;30;41m%X8[0;31;40m88[0;30;41m8S[0;33;41m@[0;31;43m8@XS@8[0;33;41m@%[0m                           [0;35;5;45;105m.........[0;1;35;95;45m;[0m             [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m                        \n"
"                        [0;1;31;91;43m8[0;31;43mS[0;33;41m8[0;31;43m@@[0;33;41m8X[0;30;41mX[0;33;41m8[0;31;43m88@8[0;33;41m88@@XX%[0;30;41m%S8[0;31;40mX@[0;30;41mX[0;33;41mX[0;31;43m88@88[0;33;41m88S[0m                         [0;1;35;95;45m:[0;35;5;45;105m........[0;1;35;95;45m;[0m              [0;1;35;95;45mt[0;35;5;45;105m.....[0m     [0;1;35;95;45m.[0;35;5;45;105m......[0m                        \n"
"                   [0;33;41mS@S;:8[0;31;43mX[0;30;41mX[0;31;40m8[0;30;41m88[0;31;40m8[0;30;41m@[0;33;41m88[0;30;41mS8[0;31;40mXSX@8[0;30;41m8[0;33;41mS@8@[0;30;41mX[0;31;40mX@[0;30;41m@S[0;33;41mX@@[0;30;41mX[0;33;41m88[0;31;43m8@[0m                        [0;35;5;45;105m........:[0m         [0;35;5;45;105m...............................[0m                  \n"
"                 [0;31;43m8[0;1;31;91;41m;t@%:[0;1;30;90;41m.[0;30;41m.;@[0;31;40mXS%S@88XSSS%%%S@[0;30;41m8@8[0;31;40mXtSSSXSS[0;30;41m@[0;33;41m88X[0;30;41mt%t;[0;33;41m%%X[0m                 [0;35;5;45;105m:........[0m          [0;35;5;45;105m...............................[0m                  \n"
"               [0;33;41m@[0;1;31;91;41m%8[0;31;5;41;101m@[0;1;31;91;41m@%[0;33;41m.[0;1;31;91;41mtX@;[0;33;41m:[0;30;41mS[0;31;40m%%tt%%t%ttttttt%%%;;;;ttS[0;30;41m8[0;31;40m88[0;30;41m8888SS%;;t:[0;33;41mtt[0m            [0;1;35;95;45m;[0;35;5;45;105m.......[0;1;35;95;45m;[0m           [0;35;5;45;105m...............................[0m                  \n"
"             [0;1;31;91;43m8[0;1;31;91;41m@[0;31;5;41;101m8SS@@t:S8[0;1;31;91;41mS:[0;33;41mt[0;31;43m@[0;33;41m@[0;31;40m%:::;;:::::.::::::::::8[0;33;41mS[0;30;41mt%%S88[0;31;40m8[0;30;41m88@XSttt[0;1;30;90;41m.[0;30;41m.:[0;33;41m%S[0m        [0;35;5;45;105m........[0m             [0;1;35;95;45mtt;;;tt;;;;;t;[0;35;5;45;105mt..............[0m                   \n"
"           [0;31;43m@[0;1;31;91;41m8[0;31;5;41;101mX;[0;1;31;91;41m8[0;31;5;41;101m8; .;@[0;1;31;91;41m@%.[0;30;41m;[0;33;41mS[0;31;43m@8[0;33;41m8[0;31;40m8t::;tt;;t;:.....:;8[0;30;41m%[0;33;41mXS[0;30;41m88%[0;1;31;91;41mt%X[0;30;41m;8@@@@@@@S;:;;[0;31;40m8[0m      [0;35;5;45;105m........[0m                            [0;35;5;45;105m.............[0;1;35;95;45m:[0m                    \n"
"          [0;33;5;41;101m8[0;31;5;41;101mX:t;:[0;33;5;41;101m.[0;31;5;41;101m.:S[0;1;31;91;41m8t:.[0;30;41mt8[0;33;41mS888@S[0;30;41m8[0;31;40mX%t;;;;:::;;%@[0;30;41m8S[0;33;41m%SS[0;30;41m%[0;31;40m@[0;30;41m8t[0;33;41m.[0;1;31;91;41m%8[0;31;5;41;101m8[0;30;41m:[0;31;40m88[0;30;41m8@@@@@@S[0;33;41m.[0;1;31;91;41m.[0;30;41m8[0m      [0;35;5;45;105m........[0m                          [0;1;35;95;45mt[0;35;5;45;105m...........:[0m                       \n"
"       [0;31;43m:[0;1;31;91;43m8[0;31;5;41;101mXttt:;;t8[0;1;31;91;41m8S;[0;30;41m :t@[0;31;40m8[0;1;30;90;41mS[0;33;41mX@@XXXXS[0;30;41mX@8[0;31;40m8@@88[0;30;41m8@@XSSS%%88;[0;1;31;91;41m;@8[0;31;5;41;101mXS[0;1;31;91;41m%[0;30;41m8[0;31;40mXX88[0;30;41m88X%:[0;1;31;91;41m::[0;30;41mX[0m      [0;35;5;45;105m.......[0m                         [0;35;5;45;105m............[0;1;35;95;45mt[0m                         \n"
"      [0;33;5;41;101mt[0;31;5;41;101m%%t:;[0;1;31;91;41m8%%SSXt:[0;30;41m.;%X[0;31;40m8X[0;30;41m@[0;33;41mSXXXXXXXSSSS[0;30;41m%%SSSXXXSXX@@%:[0;1;31;91;41m.t@[0;31;5;41;101m@SX[0;1;31;91;41m@.[0;30;41mt8[0;31;40m@X@[0;30;41m@t.[0;1;31;91;41mXSt[0;30;41mS[0m      [0;35;5;45;105m.......[0m                      [0;34;45m.[0;35;5;45;105m.............[0m                           \n"
"   [0;1;31;91;43m8[0;33;5;41;101m;[0;31;5;41;101mt:tS@8[0;1;31;91;41mS;;;ttt:[0;1;30;90;41m [0;30;41m:tS8[0;31;40mS@[0;30;41m@[0;33;41m%SXXSSSXSSXS%[0;30;41m%SSXXXX@@@@;[0;1;31;91;41m.;X88[0;31;5;41;101m@@@[0;1;31;91;41m8@8S[0;33;41m.[0;30;41mtX%t [0;1;31;91;41mSt8[0;30;41mS[0m      [0;35;5;45;105m.......[0m                     [0;35;5;45;105m:...........[0;1;35;95;45m:[0m                             \n"
"   [0;33;5;41;101m%[0;31;5;41;101m    :8[0;1;31;91;41m@%:;;;:: [0;30;41m:;%@[0;31;40m@X8[0;30;41m@%[0;33;41m%SSSS%SSS%%t[0;30;41m%SSSXX@88S[0;33;41m.[0;1;31;91;41mS8[0;31;5;41;101m88@X88[0;1;31;91;41m8X8[0;31;5;41;101m88[0;1;31;91;41m@t:..;;t@[0;30;41mX[0m      [0;35;5;45;105m........[0m                 [0;1;35;95;45m:[0;35;5;45;105m............[0;1;34;94;45m8[0m                               \n"
"   [0;31;5;41;101mt  .:t8[0;1;31;91;41mS%.::;;.[0;30;41m.:tX[0;31;40m8@[0;30;41m88XS%[0;33;41m%SSSSSS%%%t[0;30;41m%%%SX@88;[0;1;31;91;41mt%StXX888@%t@X8[0;31;5;41;101m8[0;1;31;91;41m@St%;;;X[0;30;41m@[0m      [0;35;5;45;105m........[0m               [0;1;35;95;45m;[0;35;5;45;105m............[0m                                  \n"
"   [0;31;5;41;101mX;;%88[0;1;31;91;41m8%t.:::::[0;30;41m.;S[0;31;40m8@[0;30;41m8XS%%%[0;33;41m%%S%%%ttt%%tt[0;30;41m%S@8@:[0;1;31;91;41mtS%::ttSSSt:.:S88[0;31;5;41;101m8[0;1;31;91;41mXX%S8@8[0;30;41mS[0m       [0;35;5;45;105m........[0m            [0;35;5;45;105m..............[0;1;35;95;45m;[0;34;45m.................[0m                 \n"
"   [0;1;31;91;41m8[0;31;5;41;101m888[0;1;31;91;41m88X%t.::::[0;33;41m [0;30;41m;[0;31;40m88[0;33;41m8[0;31;43m8[0;33;41m8S%%%%%SSS%[0;30;41m%%%[0;33;41mt%%%[0;30;41mtSX@8S[0;1;31;91;41m;%:...:::::.[0;30;41m.[0;1;30;90;41m [0;1;31;91;41m;X@X[0;31;5;41;101m8[0;1;31;91;41mS@@8[0;31;5;41;101m8[0;1;31;91;41m8@[0;30;41mS[0m       [0;1;35;95;45m.[0;35;5;45;105m........[0m           [0;35;5;45;105m................................[0m                 \n"
"   [0;1;31;91;41mX8@XSXtt;....[0;30;41m;@[0;31;40mXS[0;33;41m8[0;31;43m%[0;1;30;90;43m  ..[0;31;43mtX8[0;33;41m8@XXS[0;30;41mt%%[0;33;41mt%SS%SX88X.  [0;1;31;91;41m.  [0;1;30;90;41m [0;1;31;91;41m .[0;30;41m :;.[0;1;31;91;41m:%X@SX88XX8@X[0;30;41m%[0m        [0;35;5;45;105m:........[0m          [0;35;5;45;105m................................[0m                 \n"
"   [0;1;31;91;41m8Sttt%t;;:[0;30;41m.;@[0;31;40m8%%8[0;31;43mS[0;1;30;90;43m   [0;1;33;93;43m;tttttt;;[0;1;30;90;43m .[0;31;43m::[0;1;30;90;43m... .[0;31;43m;;%[0;33;41m8[0;30;41m:. [0;1;31;91;41m.[0;30;41m ...;%SX%.[0;1;31;91;41m:%S%X@XSt%@X8[0;30;41m%[0m         [0;35;5;45;105m;........[0;1;35;95;45m.[0m        [0;35;5;45;105m................................[0m                 \n"
"   [0;1;31;91;41m:t;..S%S%t[0;30;41m;[0;31;40m8888X[0;33;41m8[0;1;30;90;43m      [0;1;33;93;43m;t%%%%%%tt;;[0;1;30;90;43m      [0;31;43mS[0;33;41mt[0;30;41m..[0;1;31;91;41m.[0;33;41m [0;30;41m.;;%8[0;31;40m8[0;30;41m@;;[0;33;41m:[0;30;41m.[0;1;31;91;41m.t%SX@888@@X[0;30;41m%[0m           [0;35;5;45;105m........:[0m              [0;1;35;95;45m:[0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m                        \n"
"   [0;1;31;91;41m.;tSX88@@S;[0;30;41mX[0;31;40m8[0;30;41m8[0;31;40m8[0;30;41m8[0;31;43mS[0;1;30;90;43m    [0;1;33;93;43m;;tttttttttt;;;;[0;1;30;90;43m [0;1;33;93;43m:[0;1;30;90;43m [0;31;43m.[0;33;41mX[0;30;41m:.[0;33;41m.[0;1;31;91;41m.[0;30;41m.:::.SX%:.;:[0;1;31;91;41mt@8888@S%%@X[0;30;41mX[0m            [0;35;5;45;105m;.........[0m             [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m                        \n"
"   [0;1;31;91;41mt8[0;31;5;41;101m88@XS@8[0;1;31;91;41m88%[0;30;41m;@[0;31;40m8[0;33;41m@[0;1;30;90;43m   [0;1;33;93;43m;;;ttt%%%%%%%%tttttt:[0;31;43mS[0;33;41m:[0;30;41m.[0;1;31;91;41m...[0;30;41m..[0;33;41m.[0;1;31;91;41m.[0;30;41m.:.t%.[0;1;31;91;41m%%%@8[0;31;5;41;101m88[0;1;31;91;41mXXXt:.%[0;30;41mX[0m              [0;35;5;45;105m:.........;[0m          [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m         [0;1;35;95;45m.[0;35;5;45;105m.:[0m            \n"
"   [0;31;5;41;101m8SSSSSS@88[0;1;31;91;41m8[0;31;5;41;101m8[0;1;31;91;41m@[0;33;41m:[0;30;41m@8[0;31;43m%[0;1;30;90;43m   [0;1;33;93;43mttttt%%SS%%%%%%%%S%[0;1;31;91;43m8[0;33;41mS[0;30;41m.[0;1;31;91;41m:;:::tt... [0;30;41m:;.[0;1;31;91;41m%8[0;33;5;41;101m@S[0;1;31;91;43m8[0;31;5;41;101m88[0;1;31;91;41m8XSt[0;33;41m;;[0;1;31;91;41m;[0;30;41mS[0m                [0;35;5;45;105m;..........;[0;1;35;95;45m.[0m      [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m     [0;1;35;95;45m.[0;35;5;45;105m;.......[0;1;35;95;45m.[0m         \n"
"   [0;1;31;91;41m8[0;31;5;41;101mX@XXX@88[0;1;31;91;41mS%88%[0;30;41m:8[0;31;43m8[0;1;30;90;43m   [0;1;33;93;43m;tttttt%%%%%S%SSSS;[0;31;43m%[0;1;31;91;41m8@%%@8@@StSXStt[0;33;41m8[0;33;5;41;101mX[0;1;31;91;43m8[0;31;43m@8[0;33;41m8[0;1;31;91;41m%%%St;:[0;33;41m.[0;1;31;91;41mt[0;30;41m8[0m                  [0;34;45m.[0;1;35;95;45mt[0;35;5;45;105m............[0;1;35;95;45mt;[0;35;5;45;105m:.....[0m      [0;35;5;45;105m......[0;1;35;95;45mtt[0;35;5;45;105m............:[0m         \n"
"   [0;1;31;91;41mX[0;31;5;41;101m8@@@@8[0;1;31;91;41m8[0;31;5;41;101m8[0;1;31;91;41m8:888@[0;33;41m;@[0;1;30;90;43m    [0;1;33;93;43mt%ttttt%%%%%%%SSt[0;31;43mS[0;33;41m8@[0;1;31;91;41m8[0;31;5;41;101m8888[0;1;31;91;41m8XX888[0;33;5;41;101m8[0;1;31;91;43m88[0;31;43mS888[0;33;41m8;[0;30;41mS%%%%tt:[0;1;31;91;41m.[0;30;41m8[0m                     [0;1;30;90;45m [0;35;5;45;105mt.........................................[0m            \n"
"   [0;1;31;91;41m%[0;31;5;41;101m888[0;1;31;91;41m8[0;31;5;41;101m8[0;1;31;91;41m888@X8@88t[0;33;41m%[0;1;30;90;43m    [0;1;33;93;43mt%t%tt;[0;31;43mX@[0;1;33;93;43mt%%%%%SS[0;31;43m%8[0;33;41m888[0;33;5;41;101m8[0;31;5;41;101m888[0;1;31;91;41m8[0;31;5;41;101m88[0;33;5;41;101m%[0;1;31;91;43m88[0;31;43m%8[0;33;41m88@;[0;30;41m;;:.[0;33;41m..[0;1;31;91;41m..;tS[0;30;41mS[0m                         [0;1;35;95;45m.t[0;35;5;45;105m.................................[0;1;35;95;45m.[0m               \n"
"   [0;1;31;91;41mS888[0;31;5;41;101m8[0;1;31;91;41m8@[0;31;5;41;101m8[0;1;31;91;41m8S888@8@[0;33;41mt[0;31;43mX[0;1;30;90;43m    [0;1;33;93;43m%%%tt[0;1;30;90;43m [0;31;43m@[0;1;30;90;43m.[0;1;33;93;43m%St%S%%;[0;33;41m@[0;1;31;91;41m@[0;33;5;41;101m8[0;1;31;91;43m8[0;31;43m@888@XS%X@@@8[0;33;41m8[0;31;43m8[0;33;41m@[0;1;31;91;41mS%SSXSXXXXX@[0;30;41mS[0m                               [0;1;35;95;45m;[0;35;5;45;105mt.....................[0;1;35;95;45m::[0m                    \n"
"   [0;1;31;91;41mX@8@[0;31;5;41;101m8[0;1;31;91;41mXX88X888X%@S[0;31;43m8[0;1;30;90;43m     [0;1;33;93;43m%%[0;31;43m@[0;1;30;90;41m8[0;33;41m8[0;1;30;90;43m [0;1;33;93;43mt%%t%S%;[0;1;31;91;43m8[0;33;41m;[0;1;31;91;41m%8[0;33;5;41;101m8[0;33;41m8@XX88888@8[0;31;43m88@@8[0;33;41m8[0;1;31;91;41m8S@XX@@@@X@[0;30;41mS[0m                                   [0;35;5;45;105m.....[0m     [0;1;35;95;45m.[0;35;5;45;105m......[0m                        \n"
"   [0;1;31;91;41m@[0;31;5;41;101m88[0;1;31;91;41m88@@[0;31;5;41;101m8[0;1;31;91;41mS@@8[0;31;5;41;101m8[0;1;31;91;41m8;t8[0;33;41mS[0;31;43m;[0;1;30;90;43m   ..[0;31;43m@[0;31;40mS:[0;33;41m8[0;1;33;93;43mtt;%%%St[0;1;31;91;43m8[0;33;41m@.[0;1;31;91;41mS[0;31;5;41;101mS[0;1;31;91;43m8[0;31;43m8[0;33;41m888[0;31;43m88@@@8888@@@@8[0;1;31;91;41mS@X%@X@XSX[0;30;41m8[0m                                   [0;35;5;45;105m.....[0m      [0;35;5;45;105m......[0m                        \n"
"   [0;33;41mX[0;31;5;41;101m8888[0;1;31;91;41m@X%tt;tS%[0;30;41m.S;[0;33;41m;[0;31;43m8[0;1;30;90;43m:.::::[0;31;40mt:[0;1;30;90;41m8[0;1;30;90;43m.......:[0;31;43m8[0;30;41m:;[0;33;41m:t%X888@88888@@8[0;31;43m88@88[0;33;41mX:[0;1;30;90;41m.[0;33;41m:::::.[0;30;41mt[0;31;40m@[0m                                                                            \n";