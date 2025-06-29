#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
vector<string> hidr_frame1 = {
"**************++++*************************************+*+++********************",
"************+***+*************************************++++++**++****************",
"*****************==#%%%%#=+#***+++++***************+=+######+#%%+##*************",
"************+++++***************************************************************",
"**************+*%*+++++++##%+##++++++++**********+*##+++++*###%%=###************",
"************++*++*#####*+++##%+=+++++++*****+=++*++********++###%#++##**********",
"***++++***+=**######*+*%####====+##+++******+#%****##***+*****+===++%##*********",
"*+++++++=+===+***%*####+*%##+#++++#*++****++====*+*=+#**##%%#**+++%#*##*****++++",
"*++++++++==+==#%%##%#%+===*##*====++#++***++==#=+*#++==##+==#*#%%+===+##+**+++++",
"++++++++++++=##%==========#####+===+=*##**+#*+=+###======+====++%#====+=#*++++++",
"**++*++=+===+%##+========+%###+++===++##**#===+%%##===========#%%++====+##*+++++",
"*******+====#+*%%%==*++++++#%======+=###*+====#%%%%+==#*++++=+%#==+==+++##*+++++",
"++**++======+===#%#=#+++#+*##===#++%##**++========+#%++*++*#=#%===+#*####**+++++",
"*******=++++*===###%+=*+*%##*====+=%##***++**++====##%#=+++%###====+**%#***+++*+",
"**********=*+===+%#%%%%%%%###+==+####*****+##%+====%#%%%%%%%%#%++=+*####***+****",
"************==*=+%#%%%%=+%#%*###********+++=##%%#%%%##%%%#=%###=###****+********",
"***********+=##%##%%+%#=%##%=###***++++******##%%%###%#+%++%###*###******++*++++",
"******+*+=+###%@%%+++++###%+###**++++++*+**=+####%%%#+++++%###*###**************",
"****#==#%###%%#%%@#+++#%%%*%##*****++=%+==%####%%%%%@++++%%%%#%##***************",
"**+#==%###%%##%#%%%%%%%%%+###*******+%==*##%%%####%%%%%%%%%%+###*********++++***",
"**#==%##%%%##%##%##%%%##%=+##*******+%=+%###%#%###%##%%%%#%#=##********++++++***",
"**++###%%%##%###%##%#%%###%*+##******==%##%%%##%%###%##%%%###%=##********++++***",
"***+%##%%%##%#%%##%###%%%%%##*=##****+####%%%###%##%###%#%%%%##%++##************",
"**+###%%#%%######%##%##%%#%%%#%*=*#**=###%%#%%##%%#%#%####%%%%%##%==##**********",
"**=%##%%#%%##%##%%#*+*@%%@%%%%%%%+=+####%%#%%##%###%##++%%%%%%%%%%%%==##********",
"*+*%#%%###%#%#%#=====#%%%%%%%%%%++##=##%%##%%####%*=====%%%%%%%%%%#=*####*******",
"***+=#%#%#%###+======%%%%%%%%==####***++%###%%#%+*=====#%%%%%%%#=+####**********",
"*******=+%%%+++=====*=+++===*###******+*++=#%%#+++=====+++=====###*+************",
"*********+*++++=====*+##***************+++++=++++*======###+**++*++++***********",
"**********+*+++#===+#+*=##************+++++++=++++===++++++#**++*++++***********"};

vector<string> hidr_frame2 = {
"************+++++***************************************************************",
"**************++++*+*********************************+++*+++**+*****************",
"************+**===#*++**#+=+*********************+===++++++##%%+##**************",
"*************+*+++*#####*+#*=+=+#+++*********+===++++******++*+=+=###***********",
"**********+++#+####**+*##%++==+=##+++++*++++++%%*****##**+*#*++==++##***********",
"*********+===##**#%####%#%#+++=+=##++++=##%##===##**#==#**##%+++#%+=##**********",
"***++++**+==++####=*##%===%%*====+=#*+*+#***#==#+###+====##==*#%*===*=##********",
"*+++++++=+===*##%====*==++###%+===+++=#*****#===**##=======+++#%#====++##***++++",
"*++++++=+===*#%@==========#%%**====++****#%*===#+##===========+#=+=====###*+++++",
"*+++++++====+%#%@+==#*+++=+##=+=====+*###*#===+%#+====+#*++*==++======###*++++++",
"**++*++*====++#%@#=*+++++++%==+=++#+*==####====++#%+==*+++++=*#==+=+*###**++++++",
"*******=+===+===##%*#+*#+=#%===*+#+#######%*===+===#%#*#+*#+=#%===*+%##++*++++++",
"*+**++***+*+=+===%###++*#%#%+===+###=+%#####%#+=+==+%#%#=+*#%#%====+%##****+++++",
"***********=++===##%%%%%%%#%#++#%##*+=++++=####*+===##%%%%%%%#%*++####*****+++*+",
"************+===###%%+#%#%#%+###*************##+===%##%%+%#%%#%=###**+*****+****",
"************+=#%%%##=#%+%###+%##***************+=#%%%#%=#%+%###*%##****+********",
"#####****##%##%%%%%+++++%#%+#%#**=*#####****##%%#%%%%%++++*#%%=%%#*******++*++++",
"%%###%#######%%%%%%%++++%%#=#%#**+=%###########%%%%%@%%+++*%%#=%##**************",
"*###%%%%%%%%#%##%%%%@%%%%+=###****=###%%%%%%%%%###%%%%%%%%%%==###***************",
"=%%%%%#%%%##%##%#%%%%%%%#*+###****++%%%%%#%%%##%##%#%%%%%%%%+*###********++++***",
"=%###%%########%###%%##%%=###******=%###%%##%###%####%%%###%=#%#*******++++++***",
"=%#%%###%##%#%##%#%@%####=#%#******=%%%%##%###%#%#%##%%#####=#%#*********++++***",
"=#%##%##%#%##%%##%%%%##%#++##******=%###%%#%%##%##%#%%%#%##%+*##*******+********",
"+=%##%######%##%%##%%#%###=%##*****+=%#####%##%#%#%%##%%####*=%#****************",
"**=###%#%##%%#%%###%%###%#=###*******=#%##%##%##%%%#%#%%#####=%##***************",
"***+=#%%##%#%%%%%*+==*%##*+%##********++#%%####%%%%%+===#%#%++##+++*************",
"*******+++++++++%======%#=*%#*************+=+++++++%======%#=###+++++***********",
"********++#+++++*=====+*==#%#*********+*++++=*++++++=====*+=+%#+++++************",
"**********+++++++=====+#%#*************++++++=++++++=====+###*++*++++***********",
"***********=++++*====+=###************++++++++=+++++====+=%##*++*++++***********"};

std::vector<std::string> hidr_frame3 = {
        "************+=+=+++########*%==*===*##**********====++**#**********==++##*******",
        "*************++==+#*%++****++#+====###*********++#===#*#+#***#++*=+===*%#*******",
        "*************+=+==@%+%###%###====+#%#*********=#*##==#%++=+#*###====**%##*******",
        "************=+===#+===*##==+#+=+#=##*********+=##===#+=======+=#%=+%=#%#********",
        "**********++===##*=========++%+==++##++*******=+==+#+=========++##+===###*******",
        "*********+*==+%##%==+***====%#%+==++##+*****++===%##===+***+====##*===+###******",
        "***++++*+====+##%#=++++++#=%%###===+=##*****=+===#%%===+++++*===%%#===+=##******",
        "*++++++++++==+=*=+==*+++++=+##*+===+=###****++===++%#==#++++#==#%#+===+=#%#*++++",
        "++++++++**=#=+==%==*+====*+%#=====++##****+=*##==+=#+=++====++##===+=++###*+++++",
        "*+++++++**+*+*==*%*==+=*==%%====##+##++**+#####+#===%#==+++==###===++=###+++++++",
        "**++*+++**+=+====%##%*###%#%=====*=##*+***+++=#*+===###%*###%###====*=###*++++++",
        "*******+*****+==+*##%%%%%%#%#+=+##%#********=#*###%#+%#%%%%#%%#%#%=+####+*++++++",
        "++**++**********=*%#%%%%=%##**##**********=*#####++%*#%%%%%=%##%#++###****+++*++",
        "****************++%#%#**=####=##**********+++++*##=%###%%+*=%###=###*******+++*+",
        "****************+*%###+++##%%=#%#*****************===%##%+++*%%%+*##*+*****+*+**",
        "****************+%%%@@+++%@%=+##*******************=%%%@@#++%@%*=###***+********",
        "**************+*%#%%%%%@%@%%%+*##*+++++**********++%#%%%%%%%@%%%%+%##****++*++++",
        "************++%##%%#%%%%%%%###+##++++++*********=###%%#%%%%%%%%#%=%#************",
        "**=====++*#%%##%%#####%##%%##%=###*++======++#%%%#%%#####%###%###+*%#***********",
        "**+*%#####%%%%%#%##%#####%%###++##****=%###%#%%%%%%##%#####%#%##%#=%#****++++***",
        "***+*#%%%%%#%%##%###%####%%%###=###****+%%%%%%#%%###%###%###%%###%=*%#*++++++***",
        "****+*#####%%##%##%##%#%%%%###%=+##****++%####%%#%####%##%#%%%####+=##***++++***",
        "*****+*%#%%##%####%##%%%#%%#%#%==%#*****+=%#%%###%####%##%%#%%##%#*=%%#+********",
        "*****+=+%%###%##%##%#%%##%####%+=###*****+=%%##%####%##%%%##%%####*=%##*********",
        "*******==#%###%%####%%##%%###%#+=###******+=#%#########%%###%%#%#%#=###*********",
        "**********==#%###%%%%%##*+*%%%##=#%#*********+=*%%####%%%%#*+*%%%##=*##*********",
        "********++*++==#**++*+====#%%%%#=###************+==**+++#====+%%%%#=+%#*********",
        "********+*+++++=+++++=====+==+#####***+*++++++****=++++++====+===*#####*********",
        "*************++=*+++*====+=*##*********++++++++***++++++======+##++++***********",
        "*************++=+++++++++++=##********++++++++*****=++++*++++*=##*+++***********"
};


std::vector<std::string> hidr_frame4 = {
        "************+++=+*+##++***++********************+==*++#*=+**++******************",
        "**************=*==++***##*+**+##**+++***********+===+#*+++*+++++##***++*********",
        "**************+*=+#**########*#=#=+=+=##*********+=*+*+*********+****=+=###*****",
        "*************+*#===###*++++++++*#=+=+#%##******+=#*==#***#****++++#*+=++###*****",
        "************=+##+=+#%#%#########=+=++###******++##*==*###+=***##*#===+=###******",
        "***********+=*===*%#==+#####*##*==+=##********=%+===##+=====+#####==*##+###*****",
        "**+++++****=+===##*===++#*===+%#===+=##******+#===#++====+====+=####+=##*###****",
        "*++++++++*=====%###==========+###===++##****++===+%#+===========#*#====*=+##*+++",
        "*++++++**+=+===+%#%+=*+++++=+%###=====###***======#%+==#++++#===##%#===++##*++++",
        "*+++++++***=+====*%#=*+++*+=%@%%=====**##****++===+=%#=#++++#==+%%++===+#=##++++",
        "**++*+++****=++*==##=+**+++%##==+===++###****+*#+===+%#++**++#%#===+==+##=###+++",
        "************+++===+#%*=+#*%##*==++*+###*****+=##+====##%==*=#%##===+##*#*###*+++",
        "++**++*******+=*===%#%%%%%%%%*===*=###*****=*#***#+==+#%%%%%%%##===+#***+##+++++",
        "****************+=+*#%%*%%%##%=++*###******+=#*+=#%#%+%#%#%%%##%%%#**##+###+++*+",
        "*****************+==%#%%##+%##+###*************#+%%%#+%##%#++###%=##=#+=###++**",
        "******************+%##%#+++*%%++##**************=+=#+*#%##++++%%#==###*+*##*****",
        "******************=%%%%++++%%%%%=##*+++*************++%%%#+++#%%@%*=%#***++*++++",
        "*****************++%@%@%%%%%@@%#%*=##++*************=%%%%@%%%%%%%###=*##********",
        "****************+=%##%%%%%%%%%%###%+=#************+=##%%%%%%%%%%%###%#=##*******",
        "************+==#%####%########%%#####=##*******+=+%#####%######%#%%###%++#*++***",
        "*****+++++==#####%###%#####%###%#%#%%+=##***+=+#%#####%%%#%######%%##%##=##*+***",
        "****+++==#%###%###%%@%##%#####%%######=%#+=+%##%##%#%%%%##%##%%##%%#####=##*+***",
        "****==#%######%%%%%%%#######%%%%##%###==+%##%##%#%%%%%%##%#####%%%%#%#%#=###****",
        "*****+==%##%#%%%##%%##%##%%#%%#%##%#%+*#+=%###%%%%###%##%###%##%#%%#%###=###****",
        "********=#%%#####%%###%###%%%##%%###%=###*=+%%##%###%%####%%#%%##%%##%##=#%#****",
        "*********+=#%####%#%#%%%%%%%#%#%##%##=###***=#%###%%%##%%%%%%%#%##%%##%+=###****",
        "********++*+=========+====+*++#%%#+===###*****=========*====*+++#%%%*+==+%#*****",
        "********+*++++++++++=+====*+++++=*%###***+++++*********=====+++++++=######******",
        "*************++++++++=+===#++++++###***++++++++********=+===+++++*=###**********",
        "*************+++*+++=++===#++++++%#**+++++++++********=#+===*++++*=###**********"
};


std::vector<std::string> hidr_frame5 = {
        "************+++++***************************************************************",
        "**************++++*************************************+*+++********************",
        "************+***+**********************************++**+++++********************",
        "*******************+=+#%%%%*=+#+++++**************+#%%*######+=+##**************",
        "*****************++%#*+++++++##+**+++++***********=%%#*#*+++++*##+##************",
        "**************+=+%##+++*#####*++*=##+++********+=##%*#+++*******+++**+=*#*******",
        "**+++++******++====####%*+*######*+=***********=#====#*****+***##**+%%=###******",
        "*++++++++****++++*+###**#####%**#+===*+##********++++#*#%##**#*=#+++===++##**+++",
        "++++++++***=+====###====*##%*%###=+==+*##****=+====####===##+=++%**====++##*++++",
        "*+++++++*++====*####+==+=======%%#=+++###**++====*%*++===+======##*#*%%####+++++",
        "**++*+++*+====+=+####=========*##%*===*###*=+===+=##%+==========*#%%#===###*++++",
        "*********+=+======%*=*++++*==%%%++#====+##*+=+==+===%#=#+++**===%%%%%====+##*+++",
        "+***++******+*#===##++#+++*+#%#===+====++##***=*+===#%++#+++#+###===*====*##*++",
        "************+*====###%*+*=*%##+===++++++###**+=*====###%*++=*##%+===+++**=###+*+",
        "*************++==+###%%%%%%%#%+===+++####******++=++##%%%%%%%%#%====*%###+##+**",
        "*****************+*#%%=+%%%##%+=*==####************=####=+%%%%##*%#%%%*+=*=##***",
        "******************+####=%##%###%#*=#*++*************+%##*=%##%###%%%%##+##+**+++",
        "*******************+%#%#+++++%%@%###==*#+*************##%#++++*%%%%###*=*%******",
        "********************#%%%*+++%%%%#%%###%*==%+##*******+##%%#+++%@%%#%#%##%*==%=##",
        "********************+*%%%%%%%%%####%%####==#+##*******+#%%%%%%%%%##%#%%####==%+#",
        "******++++**********=+%##%%%#####%##%%%###=+*#%#******=+%#%%%%#####%##%%####===#",
        "****++++++********=#%##%%%##%##%#%##%%%#%%++*##*****=####%%%##########%%%%#%*###",
        "******++++*****+=###%%%%%###%##%##%#%#%%##%+##***+=###%%%%%###%##%##%#%%%%###=##",
        "*****+**+****+=###%%%#%###%#####%##%%#%%###**##==###%%%#%##%##%##%##%#%#%%##%###",
        "**********+==*%%%%%%%@%@@*+*%%##%###%##%%##%===*%%%%%%%@%%%++#%######%%%#%%##%+%#",
        "************+=+%%%%%%%%%%+====+#####%#%#%#%%+##=+%%%%%%%%%%+====+#%##%%###%#%%=#",
        "********++*+++++=+%%%%%%%%=====++%#%%###%++####**+=+%%%%%%%%=====++%##%#%#%++###",
        "********+*++++++++====+++=+=====+++%%%=+###+++******+====++=======+++%%#=+#%##**",
        "*************+++++++++***=+====++++++###+++++++***********+=*====++++++####*****",
        "*************+++*++++++=*+*+===*++++*##*++++++**********+=*+*+===*++++#%#*******"
};

std::vector<std::string> hidr_frame6 = {
    "************+++++***************************************************************",
    "**************++++********++***************************+*+++++******************",
    "************+***+*+=+##**++#===+********************#%%**+++++++++**************",
    "**************+=+=+#++######+++#+#*+*************=*=+*++*******++*===+**********",
    "**************+#==++%##*++*####**++++++**********+==+++***+**##***+%%+**+++*****",
    "*************=+=+++*%%#####%%**##===+##********=+==++=%%#**#+=****#+==*#%##+###*",
    "**+++++****=+====+%@===%###=+%##=#==+###*****+*=====##==+#+====%%#+#==****#+##**",
    "*++++++++*=+===+%##%++==*====#%%#===+=##****++====*##*+++======+*##%%%##****#=++",
    "++++++++*+=====**%%#==========%%##===+=##***++====+##============#*#===+##*****#",
    "*+++++++***=+==+=+##=*++*#+==%%##=+===+#%#***=*=====#==#++*#*====+*%%===**###*#=",
    "**++*+++****++=*==##++++++*=#@%#++====**##*****+++==*#=+++++*===%#*++===+###+=+=",
    "*************++===%#=+#*+#*#%%===+===+=###*****=++==#%++#*+*##%#===+====%#######",
    "+***++*******===-=%#%#+*=###%+=-==+*+####******=+===##%%+*=*%###===+=#%%%#%#%%=+",
    "***************++*%#%%%%%%%##====*+###***********+++##%%%%%%%#%+===#####=+++++=#",
    "*****************+%#%%*%+%%#%%===+###**************+##%%+%+#%%#%+==+#######*****",
    "*****************+###%=%%=%#%%%#=*#******************%##=#%=%##%%%++#**+********",
    "******************=%###++++%%%%%##%##****######=###*=####++++%%%%%%#%##****#####",
    "******************=#%%#+++#%%%%%%########%###%=*##**++%%#+++#%%%%%%####%########",
    "*******************==%%%%@%%%%%##%%%%%%%%#####=###***==%%%%%@%%%%##%%%%%%%%%###%",
    "********************+%%%%%%%%####%##%%%#%#%%%=#%#*****=%%%%%%%%######%#%%##%%%%*",
    "******++++**********=###%%%###%###%###%%%###%=###*****+####%%####%#######%%####+",
    "****++++++**********=%####%%##%###%######%%%%=%##*****=%#%##%%%##%##%###%##%%##+",
    "******++++*********++%#%##%%%%##%##%#%##%##%#=%##*****=%#%#%%%%####%##%###%###%=",
    "*****+**+******++++=###%#%%##%%##%#########%+####****+=####%%##%%####%##%#####++",
    "**********+***+++++=#####%%###%%###%##%###%=#%#******+=#%##%%##%#%%##%##%###%=#%",
    "************+++++++++%#%#===+%%%%%##%#%%#+*###********=%#%#+==+#%%%%####%%#+=%%#",
    "********++*+++++++++=#%======%*+++++*+*#####**********=%%*=====+%+++++*=+######*",
    "********+*++++++++++==+#=====*+++++*+###++++++********+=+#======+++++++###******",
    "*************+++++++++*======+++++++###++++++++**********++=====++++++#%#*******",
    "*************+++*+++++*=+====*++++=###++++++++***********++=====++++*+###*******"
};

std::vector<std::string> hidr_frame7 = {
    "*********=+==+=##*#######+*+=+=*##**********+===++*#+*****#**+*+=+=###**********",
    "*********=*==++++****++##*+==++###***********====+*++#***#+#*#===#*+##**********",
    "**********=====%##%###%*#@*==++##************++====###**#=++##+=+###=##*********",
    "**********=++==@#==+##===+*+==++##++**********=++=+#++=======+*#%#+##+%#********",
    "*********+*===%#+=========+##===++#*+++******=+==+#++==========#*===+###********",
    "********++===%#%====+***==#%###==++#*++*****=+===%##====+*#+===++%+==**##*******",
    "***++++++===*##%@+++++++#=*%#%*===++##*****=+===+%%#==#+++++===+%%+====+##******",
    "*++++++=+===++##*==*+++++=+++=+==+=###*****=+===+*%%==+++++*==*%#=+==#=###**++++",
    "++++++++++==+===%**+====#==%+=+=+=##*++*****++==+==#%+*=====*==%==+=%*#=+##+++++",
    "*+++++++*++#====#%+=*=+===%#==+#+=##*++******+##====%%==+=+==+%#==++#####=##++++",
    "**++*+++*+++====##%%##*#%#%+===+++###*+******=*=====%#%###*%##%====*#+=+*###++++",
    "***********++=+*##%%%%%%###=+=*####*+**********+=###%#%%%%%%###+#%##*#=###++*+++",
    "+****+********+=%##*%%%%##*+*###****************+=+%##%=%%%%#%#%%=#%####=*#++*++",
    "**************++%#%++**%%%*+###*******************=####=**#%#%#%#*##++++*##*++*+",
    "**************=%###+++*##%#=%##******************+=%%#%+++##%%#==###*+*****+*+**",
    "**************+=%%%*++%%@%%####*******************==%@%+++@@@%%+###****+********",
    "*************+=%%%@%%@%%%%%%#+##**+++++**********++%%%@%%%%%%%#%**##*****++*++++",
    "*************+=##%%%%%%%%#%%#%++#*+++++**********+*##%%%%%%%#%%##%+*#***********",
    "*************=####%########%%##%%#*++=====*******=%##%%#%######%%#%%%#*++=====**",
    "************++%#%#%#%###%###%%%%%%#######+###***++###%%####%#####%%%%####%#%*###",
    "*****+++++**=*####%###%#####%#%%#%%%%%%#+%##****=##%#%%#%###%##%##%%#%%%%%##+###",
    "****++++++**=#####%%#%##%#%###%#%%####%+###****+=%#%#%%%%#%########%%##%##*+###*",
    "******++++*+=##%##%#%%##%####%##%%%##%=###*****==%###%%#%%##%#%##%###%%#%*+###**",
    "*****+**+**+=%###%%###%##%#%#######%#=###******=+%####%##%%####%###%##%%++###***",
    "**********+==%##%#%##%#%####%#%##%%+=###*******=+##%##%###%%#%##%###%%#==###****",
    "***********=+%%%%#++##%%%%####%#+=*####********=*#%%%#+*##%%%###%#%#=+####******",
    "********++*=+%%%%%=====%++***=+####************=*%%%%#====+*++**#=+####*********",
    "********+*+++++===+====*++++*+###*****+*++++++****+==+=====+++++=###************",
    "*************++++=+====*++++=###*******++++++++*****+=+====*+++*=##*+***********",
    "*************++++++++++*++++=###******++++++++******=++++++++++*+##++***********"
};

std::vector<std::string> hidr_frame8 = {
    "************+++++**+***+=**+#+=+#**********************++**+=+*=+*==#***********",
    "*********+++**++++*+**#***++==++###**********+++****++++++++++*#==+=%##*********",
    "********=+=+=#+#*########**#==++##**********+===+**#+*********+#=++=#%#*********",
    "********++=+=#+++++++++*###+==##+#++*********===+#*+++****+#***#==*#=##*********",
    "*********++=+=*#########*%#+=+##+=##+++******+==+==#*#**#*=+###*=+%##=###*******",
    "*********+=+===########*==*%#===#=+##++*****++##===#####+=====+#%#==+#+##*******",
    "***++++**++===##+===*#*+===+##+====*#******+*#%===+#+=+====*====+++===++##******",
    "*+++++++=+===*##+==========*##%===++##*****==*====#*#===========+#%+===+###*++++",
    "++++++++====+###%*=+*++++=+%#%+===+=###****+*+===%%#%===*+++++==#%*===+=+##*++++",
    "*++++++=+====+#%@%+=*++++=#@#====+=###+***+=#====+*%%+==*+++++=%%=+====*##*+++++",
    "**++*+++++===+==##%++***+=%#==+=+=###*+***+=##===+==+#%*++**+=##==+=+#+##*++++++",
    "**********=#++==###%*++=*%%+===+++%#+******+***#*+===%###++=+###====*##=##++++++",
    "****++*****=#===*#%%%%%%%##===+=####********=#**#+===%%%%%%%%%%+==##**##*=#*++++",
    "************+++=##%#%%#%###+=+####**********++##*##%%##%%%#%%##=%###=+#*=###*+*+",
    "**************++%#%=##%##%*=###*************+=+*=##=##%***%%###*#%%#+##*###**+**",
    "*************++=%%#+++*###%+###****************#*+==#%%++++%#%%*##=+=##+********",
    "************+=%%%@%++++%%%#=%%#****++++**********=##%%%++++%%%%=###******++*++++",
    "***********=*%##%@%%%%@%%%%#+##**++++++********==%##%@%%%%%@%%%#=##*************",
    "*********=+%##%%%%%%%%%%%##%=+##***++++******+=%####%%%%%%%%%%#%+=##************",
    "********=#####%%#######%%####%#=+***********=+##%##%%#######%%#####+=*#**+++****",
    "*****++=+%##%%%#%###%###%%##%###%#==*#*****+=%##%#%%###%##%%#%######%#==+*+++***",
    "****+++=*#####%%##%##%%##%%%##%####%#==+#**+=%####%%#%###%##%%%%#%#####%%+=+****",
    "******+=*#%##%%%%######%##%%%%%##%######==#*=%#%#%%%%##%######%%%%%%%####%#%==##",
    "*****+*++###%%%#%%#%###%##%%##%%%%##%%+=###*=%###%%%%%###%####%%##%%%%####*=*###",
    "*******+=%###%%##%%#%######%#%####%%#=####**=#%###%##%%#%##%##%%%####%%%%=+###**",
    "********=##%#%%###%%%%%%%###%%#%###+*###****=*##%#%##%%%%%%%%%##%%#%##%++#%#****",
    "********===+#%%#+++===============*###******===+*%%%*++======+========+#%#******",
    "********+*+*+=++++++====++###*********+*++++++***=#++++*====*=##*+++************",
    "*************++++++#=====#%#***********++++++++**+=++++#====++##*++++***********",
    "*************++++++*===++=*##********+++++++++****=++++#+==++++##++++***********"
};

// Now you have hidr_frame1
