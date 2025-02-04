#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include <fstream>
#include <regex>
using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
bool DARK_LIGHT = true;
string CourentFile;
bool Mode_File = true;
bool toggleFile = false;
string Error = "";
string Error2 ="";
string Error3="";
string Error4="";
string Error5="";
vector<string> Errors;
vector <string> filenames;
string undochar = "";
vector <bool> undobool;
vector <vector<int>> undopos;
int UndoPointer = -1;
bool undoredocurrentcheker = false;
int currentLine = 0; // Track the current line being edited
int cursorPos= 0; // Track the cursor position within the current line
bool CntG = false;
string FolderName = "";
void OpeningURL(string url);
void LoadingAnimation (SDL_Renderer* r);
void compiler (vector <string> codes);
int run_in_another_window(const std::string &filename);
void MakingBg1 (SDL_Renderer* r);
void MakingBg2 (SDL_Renderer* r);
void UploadSvg (SDL_Renderer * r, const char *path , int x , int y , int w , int h);
void UploadTxt (SDL_Renderer * r, string text , TTF_Font* f , SDL_Color c, int x , int y );
void ensureLastLineVisible(int currentLine, int &scrollOffset, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines);
void NewFile(string filename);
void RenderPage(SDL_Renderer * renderer, SDL_Texture * bg, int scrollOffset, vector<string>lines , int LINE_HEIGHT,TTF_Font * font,int currentLine,int cursorPos, SDL_Color textColor);
void ErrorHandler(SDL_Renderer* r,string s);
bool libcommandcheker(SDL_Renderer *r , vector<string> a);
bool libincludecheker (string s ,  vector<string> a);
void UndoRedo(vector<string> &a);
string AutoCompletChheker(string s);
void handleKeyPress(SDL_Event& e);
void handleMouseEvent(SDL_Event& e);
bool hasUnclosedQuotes(const string& line);
bool checkUnclosedQuotes(SDL_Renderer *r, vector<string> lines);
void UnableVariableNaming(vector<pair<string, SDL_Color>> &words);
void SemicolonErrors(vector<pair<string, SDL_Color>> &words);
void OperandErrors(vector<pair<string, SDL_Color>> &words);
bool semicoloncheker = false;
bool inMultilineComment=false;
bool checkReservedKeywordUsage(SDL_Renderer *r, vector<string> lines);
bool ParenthesesErrors(SDL_Renderer* r, vector<string> lines);
bool CheckSpellingErrors(const vector<string>& lines, string& Error5);
void getUndefindvars(vector<string> code);

int calculateXForPosition(int pos, int line) {
    return pos * 10; // فرض کنید CHAR_WIDTH عرض هر کاراکتر است
}

int calculateYForLine(int line) {
    return line * 20; // فرض کنید LINE_HEIGHT ارتفاع هر خط است
}
void renderCharWithColor(char ch, int x, int y, SDL_Color color) {
    // استفاده از SDL_ttf یا روش‌های دیگر برای رندر کاراکتر با رنگ خاص
    // این بخش نیاز به پیاده‌سازی دارد.
}
std::vector<std::string> extractVariables(const std::string& code);
std::vector<std::string> variables;


SDL_Color dark_blue = {0, 51, 102, 255};
SDL_Color green_blue = {0, 128, 128, 255};
SDL_Color dark_orange = {255, 140, 0, 255};
SDL_Color dark_red = {139, 0, 0, 255};
SDL_Color dark_green = {0, 100, 0, 255};
SDL_Color purple = {128, 0, 128, 255};
SDL_Color comment = {128, 128, 128, 255};
SDL_Color sky_blue = {0, 139, 139, 255};
SDL_Color cherry = {128, 0, 0, 255};
SDL_Color golden = {184, 134, 11, 255};
SDL_Color light_purple = {221, 120, 198, 255};
SDL_Color red = { 224, 108, 117,};
SDL_Color light_blue = {97, 175, 254, 255};
SDL_Color yellow = {229, 192, 123, 255};
SDL_Color light_green = {152, 195, 121, 255};
SDL_Color orange = {209, 154, 102, 255};
SDL_Color comment2 = {92, 99, 112, 255};
SDL_Color firouz_blue = {86, 182, 194, 255};
SDL_Color dark_orange2 = {213, 94, 0, 255};
SDL_Color light_black = {171, 178, 191, 255};
SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Color SPACE_COLOR = {150, 150, 150, 255};
SDL_Color Error_Color = {255, 0, 0, 255};
SDL_Color blue = {0, 100, 255, 100};

vector<string> lines;
string clipboardText;// Holds clipboard text
int lineHeight = 0;
// Variables for mouse selection
int selectionStartLine = -1;
int selectionStartPos = -1;
int selectionEndLine = -1;
int selectionEndPos = -1;
bool selection= false;

// Function prototypes
std::vector<std::pair<std::string, SDL_Color>> splitLineIntoWords(const std::string& line, bool &inMultilineComment);
SDL_Color getKeywordColor(const std::string &word);

int main(int argc , char * argv[]) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) { // Or other formats like IMG_INIT_JPG
        std::cerr << "SDL_image Initialization Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }



    // Create window
    SDL_Window *window = SDL_CreateWindow("MY code",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    system(R"(dir /b "..\" /ad > folders.txt)");
    ifstream in2("folders.txt");
    if (in2.good()){
        string gb = ".idea";
        while (gb == "cmake-build-debug" || gb == ".idea" ){
            in2 >> gb;
        }
        FolderName = gb;

    }

    // Load font
    TTF_Font *font = TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18); // Replace with the path to your .ttf font
    vector<string> lines; // Holds multiple lines of text

    int scrollOffset = -40; // Keeps track of scrolling
    const int LINE_HEIGHT = TTF_FontHeight(font); // Height of each line
    SDL_Color textColor = {0, 0, 0, 255};
    ifstream in("mem\\prevousFILE.txt");
    if (in.good()){
        in >> CourentFile;
    } else{
        cerr << "error finding previos file... ";
    }
    in.close();

    if (!CourentFile.empty()){
        // CourentFile.insert(CourentFile.begin(), ' ');
        ifstream in3("..\\" + FolderName + "\\" + CourentFile);
        if (in3.good()){
            string gb;
            while (!in3.eof()){
                getline(in3,gb);
                lines.push_back(gb);
            }
        } else{
            cerr << "error opening previos file...1 ";
        }
        in3.close();
    }



  //  LoadingAnimation(renderer);

    // Timer for cursor blinking
    Uint32 lastCursorToggle = SDL_GetTicks();
    bool cursorVisible = true;
    const Uint32 CURSOR_BLINK_INTERVAL = 500; // 500 ms for blinking

    //making auto commplet map...
    bool matchcomplet = false;
    string patern = "";
    map<string,string> mp;
    mp["if"] = "if () {}";
    mp["else"] = "else {}";
    mp["while"] = "while {}";
    mp["switch"] = "switch (case) { }";
    mp["int"] = "int ";
    mp["float"] = "float ";
    mp["double"] = "double ";
    mp["string"] = "string ";
    mp["char"] = "char ";
    mp["long long"] = "long long ";
    mp["main"] = "main () ";
    mp["cout"] = "cout << ";
    mp["cin"] = "cin >> ";
    mp["endl"] = "endl; ";
    mp["pow"] = "pow (a,b)";
    mp["abs"] = "abs()";
    mp["min"] = "min(a,b)";
    mp["max"] = "max(a,b)";
    mp["printf"] = "printf()";
    mp["scanf"] = "scanf()";
    mp["strlen"] = "strlen()";
    mp["for"] = "for(auto i : x) {} ";
    mp["#include"] = "#include <MY_code>";
    mp["using"] = "using namespace std;";


    //making background components...
    SDL_Texture * bg = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    SDL_SetRenderTarget(renderer,bg);

    MakingBg1(renderer);


    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

    SDL_RenderPresent(renderer);


    //start events
    SDL_Event e;



    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);


    //event loop handling...start
    bool quit = false;
    while (!quit) {


        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastCursorToggle + CURSOR_BLINK_INTERVAL) {
            cursorVisible = !cursorVisible;
            lastCursorToggle = currentTime;
        }

        while (SDL_PollEvent(&e) != 0) {

            if (e.type == SDL_QUIT) {
                quit = true;
            }  else if (e.type == SDL_MOUSEWHEEL) {
                // Handle scroll
                if (e.wheel.y > 0) { // Scroll up
                    //   scrollOffset = std::max(0, scrollOffset - LINE_HEIGHT);
                    scrollOffset -= LINE_HEIGHT;
                } else if (e.wheel.y < 0) { // Scroll down
                    scrollOffset += LINE_HEIGHT;
                }
                RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
            }
            else if (e.key.keysym.sym == SDLK_q && SDL_GetModState() & KMOD_CTRL) {
                // شروع انتخاب با Ctrl + Q
                selectionStartPos = cursorPos;
                selectionStartLine = currentLine;
                std::cout << "Selection started at (" << selectionStartLine << ", " << selectionStartPos << ")\n";
                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                UploadTxt(renderer,"Selection started at (" + to_string(selectionStartLine) + ", " + to_string(selectionStartPos) + ")",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18),{255,255,255,255},150,45);
                SDL_RenderPresent(renderer);
            }
            else if (e.key.keysym.sym == SDLK_p && SDL_GetModState() & KMOD_CTRL) {
                // پایان انتخاب با Ctrl + P
                selectionEndPos = cursorPos;
                selectionEndLine = currentLine;
                std::cout << "Selection ended at (" << selectionEndLine << ", " << selectionEndPos << ")\n";


                // اطمینان حاصل کنید که شروع قبل از پایان است
                if (selectionStartLine > selectionEndLine ||
                    (selectionStartLine == selectionEndLine && selectionStartPos > selectionEndPos)) {
                    std::swap(selectionStartLine, selectionEndLine);
                    std::swap(selectionStartPos, selectionEndPos);
                }

                // چاپ متن انتخاب شده
                if (selectionStartLine == selectionEndLine) {
                    // اگر انتخاب در یک خط است
                    std::string selectedText = lines[selectionStartLine].substr(selectionStartPos, selectionEndPos - selectionStartPos);
                    std::cout << "Selected text: " << selectedText << "\n";
                } else {
                    // اگر انتخاب چند خطی است
                    std::string selectedText;
                    selectedText += lines[selectionStartLine].substr(selectionStartPos) + "\n"; // از شروع تا انتهای خط اول
                    for (int i = selectionStartLine + 1; i < selectionEndLine; ++i) {
                        selectedText += lines[i] + "\n"; // خطوط کامل بین خط اول و آخر
                    }
                    selectedText += lines[selectionEndLine].substr(0, selectionEndPos); // از ابتدای خط آخر تا پایان انتخاب
                    std::cout << "Selected text:\n" << selectedText << "\n";
                }

                // فعال کردن حالت انتخاب
                selection = true;

                // هایلایت متن انتخاب شده
                for (int lineIdx = selectionStartLine; lineIdx <= selectionEndLine; ++lineIdx) {
                    int start = (lineIdx == selectionStartLine) ? selectionStartPos : 0;
                    int end = (lineIdx == selectionEndLine) ? selectionEndPos : lines[lineIdx].length();

                    // رندر متن انتخاب شده با رنگ آبی
                    for (int charIdx = start; charIdx < end; ++charIdx) {
                        // محاسبه موقعیت X و Y برای هر کاراکتر
                        int charX = calculateXForPosition(charIdx, lineIdx);
                        int charY = calculateYForLine(lineIdx);

                        // رندر کاراکتر با رنگ آبی
                        renderCharWithColor(lines[lineIdx][charIdx], charX, charY, blue);
                        boxRGBA(renderer, charX, charY, charX+10, charY+20, 100, 100, 100, 50);
                    }
                }
                RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                // به‌روزرسانی صفحه
                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                UploadTxt(renderer,"Selection ended at (" + to_string(selectionEndLine) + ", " + to_string(selectionEndPos) + ")",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18),{255,255,255,255},150,45);
                SDL_RenderPresent(renderer);


            }
//            else if (e.type == SDL_MOUSEBUTTONDOWN ) {
//                // Start new selection when mouse button is pressed
//                int mouseX = e.button.x;
//                int mouseY = e.button.y;
//
//                // Calculate the line and character position based on mouse coordinates
//                selectionStartLine = mouseY / lineHeight;  // Example: calculate line based on mouseY
//                selectionStartPos = mouseX / charWidth;    // Example: calculate character position based on mouseX
//
//                // Ensure the selection start is within bounds
//                selectionStartLine = std::max(0, std::min(selectionStartLine, (int)lines.size() - 1));
//                selectionStartPos = std::max(0, std::min(selectionStartPos, (int)lines[selectionStartLine].size()));
//
//                // Move the cursor to the start position
//                cursorPos = selectionStartPos;
//                currentLine = selectionStartLine;
//
//                std::cout << "Selection started at (" << selectionStartLine << ", " << selectionStartPos << ")\n";
//            }
//
//            else if (e.type == SDL_MOUSEMOTION && selectionStartLine != -1) {
//                // Update selection as mouse moves
//                int mouseX = e.motion.x;
//                int mouseY = e.motion.y;
//
//                // Calculate the line and character position based on mouse coordinates
//                int selectionEndLine = mouseY / lineHeight;
//                int selectionEndPos = mouseX / charWidth;
//
//                // Make sure the calculated positions are within bounds
//                selectionEndLine = std::max(0, std::min(selectionEndLine, (int)lines.size() - 1));
//                selectionEndPos = std::max(0, std::min(selectionEndPos, (int)lines[selectionEndLine].size()));
//
//                // Update the cursor position and current line
//                cursorPos = selectionEndPos;
//                currentLine = selectionEndLine;
//
//                std::cout << "Selection updated to (" << selectionEndLine << ", " << selectionEndPos << ")\n";
//            }
//
//            else if (e.type == SDL_MOUSEBUTTONUP) {
//                // End selection when mouse button is released
//                int mouseX = e.button.x;
//                int mouseY = e.button.y;
//
//                // Calculate the line and character position based on mouse coordinates
//                selectionEndLine = mouseY / lineHeight;
//                selectionEndPos = mouseX / charWidth;
//
//                // Ensure the selection end is within bounds
//                selectionEndLine = std::max(0, std::min(selectionEndLine, (int)lines.size() - 1));
//                selectionEndPos = std::max(0, std::min(selectionEndPos, (int)lines[selectionEndLine].size()));
//
//                std::cout << "Selection ended at (" << selectionEndLine << ", " << selectionEndPos << ")\n";
//
//                // Handle the selection range
//                if (selectionStartLine == selectionEndLine) {
//                    // Selection is on the same line
//                    if (selectionStartPos > selectionEndPos) {
//                        std::swap(selectionStartPos, selectionEndPos); // Ensure start is before end
//                    }
//                } else if (selectionStartLine < selectionEndLine) {
//                    // Start is above the end, swap if necessary
//                    if (selectionStartPos > lines[selectionStartLine].size()) {
//                        selectionStartPos = lines[selectionStartLine].size();
//                    }
//                    if (selectionEndPos > lines[selectionEndLine].size()) {
//                        selectionEndPos = lines[selectionEndLine].size();
//                    }
//                } else {
//                    // If the selection starts below the end
//                    if (selectionEndPos > lines[selectionEndLine].size()) {
//                        selectionEndPos = lines[selectionEndLine].size();
//                    }
//                    if (selectionStartPos > lines[selectionStartLine].size()) {
//                        selectionStartPos = lines[selectionStartLine].size();
//                    }
//                }
//
//                // Update cursor position to the end of the selection
//                cursorPos = selectionEndPos;
//                currentLine = selectionEndLine;
//
//                std::cout << "Cursor position updated to (" << currentLine << ", " << cursorPos << ")\n";
//            }

            else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() && KMOD_CTRL) {
                // Ctrl + V: Paste the clipboard text at the cursor position or replace selection
                char* clipboardText = SDL_GetClipboardText();
                if (clipboardText) {
                    if (selectionStartLine != -1 && selectionEndLine != -1 && selection) {
                        if (selectionStartLine == selectionEndLine) {
                            // If the selection is within a single line
                            lines[selectionStartLine].erase(selectionStartPos, selectionEndPos - selectionStartPos);
                        } else {
                            // Remove the text from the first line (selectionStartLine)
                            lines[selectionStartLine].erase(selectionStartPos);

                            // Remove the text from the last line (selectionEndLine)
                            lines[selectionEndLine].erase(0, selectionEndPos);

                            // Remove the entire lines in between (if any)
                            for (int line = selectionStartLine + 1; line < selectionEndLine; ++line) {
                                lines[line].clear();  // Clear the whole line
                            }
                        }

                        // Now replace the selection (or insert at cursor position)
                        lines[selectionStartLine].insert(selectionStartPos, clipboardText);

                        // Update cursor position to the end of the pasted text
                        cursorPos = selectionStartPos + strlen(clipboardText);

                        // Clear selection after pasting
                        selectionStartLine = -1;
                        selectionEndLine = -1;
                    } else {
                        // No selection, just insert clipboard text at cursor position
                        lines[currentLine].insert(cursorPos, clipboardText);
                        cursorPos += strlen(clipboardText);
                    }

                    // Free the clipboard text
                    SDL_free(clipboardText);

                    // Render the page with the new text
                    RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                    selection = false;
                }
            }
            else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                // Ctrl + C: Copy the selected text to clipboard
                if (selectionStartLine != -1 && selectionEndLine != -1) {
                    std::string selectedText;
                    if (selectionStartLine == selectionEndLine) {
                        selectedText = lines[selectionStartLine].substr(selectionStartPos, selectionEndPos - selectionStartPos);
                    } else {
                        // Handle multi-line selection
                        selectedText = lines[selectionStartLine].substr(selectionStartPos) + "\n";
                        for (int i = selectionStartLine + 1; i < selectionEndLine; ++i) {
                            selectedText += lines[i] + "\n";
                        }
                        selectedText += lines[selectionEndLine].substr(0, selectionEndPos);
                    }
                    // Copy the selected text to the system clipboard
                    SDL_SetClipboardText(selectedText.c_str());

                }
                RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                UploadTxt(renderer,"The Selected text copied to Clipboard!",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18),{255,255,255,255},150,45);
                SDL_RenderPresent(renderer);
                selection = false;
            }
            else if (e.key.keysym.sym == SDLK_x && SDL_GetModState() & KMOD_CTRL) {
                // Ctrl + X: Cut the selected text to clipboard and delete it
                if (selectionStartLine != -1 && selectionEndLine != -1) {
                    // Copy the selected text to clipboard
                    std::string selectedText;
                    if (selectionStartLine == selectionEndLine) {
                        selectedText = lines[selectionStartLine].substr(selectionStartPos, selectionEndPos - selectionStartPos);
                    } else {
                        // Handle multi-line selection
                        selectedText = lines[selectionStartLine].substr(selectionStartPos) + "\n";
                        for (int i = selectionStartLine + 1; i < selectionEndLine; ++i) {
                            selectedText += lines[i] + "\n";
                        }
                        selectedText += lines[selectionEndLine].substr(0, selectionEndPos);
                    }
                    // Copy the selected text to the system clipboard
                    SDL_SetClipboardText(selectedText.c_str());
                    if (selectionStartLine == selectionEndLine) {
                        // Single line selection
                        clipboardText = lines[selectionStartLine].substr(selectionStartPos, selectionEndPos - selectionStartPos);
                        lines[selectionStartLine].erase(selectionStartPos, selectionEndPos - selectionStartPos);
                    } else {
                        // Multi-line selection
                        clipboardText = lines[selectionStartLine].substr(selectionStartPos) + "\n";
                        for (int i = selectionStartLine + 1; i < selectionEndLine; ++i) {
                            clipboardText += lines[i] + "\n";
                        }
                        clipboardText += lines[selectionEndLine].substr(0, selectionEndPos);

                        // Delete the selected text
                        lines[selectionStartLine].erase(selectionStartPos); // Remove the selected part from the first line
                        lines[selectionEndLine].erase(0, selectionEndPos);  // Remove the selected part from the last line

                        // Remove the lines in between (if any)
                        if (selectionEndLine > selectionStartLine + 1) {
                            lines.erase(lines.begin() + selectionStartLine + 1, lines.begin() + selectionEndLine);
                        }
                        selection = false;
                    }

                    // Reset cursor position to the start of the selection
                    cursorPos = selectionStartPos;
                    currentLine = selectionStartLine;

                    // Ensure cursorPos is within bounds
                    if (cursorPos > lines[currentLine].size()) {
                        cursorPos = lines[currentLine].size();
                    }

                    // Clear selection after cutting
                    selectionStartLine = -1;
                    selectionEndLine = -1;

                    // Render the page with the updated text
                    RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                    cout << 3;
                }
            }
            else if (e.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL) {
                // Ctrl + A: Select all text
                selectionStartLine = 0;
                selectionEndLine = lines.size() - 1;
                selectionStartPos = 0;
                selectionEndPos = lines[selectionEndLine].size();
                cursorPos =0;
                currentLine=0;
                RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                UploadTxt(renderer,"Selected All lines from file : " + CourentFile ,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18),{255,255,255,255},150,45);
                SDL_RenderPresent(renderer);
                selection = true;
            }
            else if (e.key.keysym.sym == SDLK_z && SDL_GetModState() & KMOD_CTRL) {
                // Ctrl + Z: Undo the last action (Placeholder)
                UndoRedo(lines);
                UndoPointer--;
                undoredocurrentcheker = true;
                cursorPos = 0; // Reset cursor to the beginning (you can enhance this further)
                selectionStartLine = -1;
                selectionEndLine = -1;
                RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                selection = false;
            }
            else if (e.key.keysym.sym == SDLK_g && SDL_GetModState() & KMOD_CTRL) {
                // Ctrl + G: Go to line (you can add user input handling here)
                bool exit = false;
                string ln;
                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                UploadTxt(renderer,"Go to line : " + ln,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{255,255,255,255},150,45);
                SDL_RenderPresent(renderer);
                while (!exit) {
                    while (SDL_PollEvent(&e) != 0) {
                        if (e.type == SDL_TEXTINPUT) {
                            if (e.text.text) {
                                ln+=e.text.text;
                                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                                UploadTxt(renderer,"Go to line : " + ln,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{255,255,255,255},150,45);
                                SDL_RenderPresent(renderer);

                            }
                        } else if (e.type == SDL_KEYDOWN){
                            if (e.key.keysym.sym == SDLK_BACKSPACE){
                                ln.erase(ln.size() - 1);
                                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                                UploadTxt(renderer,"Go to line : " + ln,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{255,255,255,255},150,45);
                                SDL_RenderPresent(renderer);
                            }else if (e.key.keysym.sym == SDLK_RETURN){
                                exit = true;
                            }
                        }


                    }
                }
                int lineNumber = stoi(ln);
                std::cout << "Enter line number: ";
                //std::cin >> lineNumber;

                if (lineNumber >= 0 && lineNumber < lines.size()) {
                    currentLine = lineNumber - 1;
                    cursorPos = 0;  // Reset cursor to start of the line (this can be modified)
                    selectionStartLine = -1;
                    selectionEndLine = -1;
                    ensureLastLineVisible(currentLine,scrollOffset,SCREEN_HEIGHT,LINE_HEIGHT,lines.size());
                    CntG = true;
                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

                } else {
                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                    boxRGBA(renderer,144,40,800,70,100,100,100,255);
                    UploadTxt(renderer,"The line : " + ln + " is undefined!",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18),{255,255,255,255},150,45);
                    SDL_RenderPresent(renderer);
                }
            }
            else if (e.key.keysym.sym == SDLK_s && SDL_GetModState() & KMOD_CTRL) {
                ofstream out("..\\" + FolderName + "\\" + CourentFile );
                if(out.good()){
                    for (auto i : lines) {
                        out << i << endl ;
                    }
                }
                out.close();
                RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                cout << "file saved";
                boxRGBA(renderer,144,40,800,70,100,100,100,255);
                UploadTxt(renderer,"The file : " + CourentFile + " saved succesfuly!",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18),{255,255,255,255},150,45);
                SDL_RenderPresent(renderer);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN){
                if (e.button.x > 760 && e.button.x < 788 && e.button.y > 3 && e.button.y < 33 )
                    quit = true;
                else if (e.button.x > 600 && e.button.x < 625 && e.button.y > 8 && e.button.y < 33 ){
                    //Redo algorithm...
                    if (undoredocurrentcheker && UndoPointer < undochar.length() - 1){
                        UndoPointer++;
                        UndoRedo(lines);
                        RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                    }
                }else if (e.button.x > 570 && e.button.x < 595 && e.button.y > 8 && e.button.y < 33 ){
                    //Undo algorithm...
                    UndoRedo(lines);
                    UndoPointer--;
                    undoredocurrentcheker = true;
                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                    cout << 1;

                }else if (e.button.x > 80 && e.button.x < 118 && e.button.y > 1 && e.button.y < 40 ){
                    OpeningURL("https://github.com/zxd1385/MY_code__IDE");
                }else if (e.button.x > 642 && e.button.x < 667 && e.button.y > 10 && e.button.y < 35 ){
                    Errors.clear();
                    checkUnclosedQuotes(renderer, lines);
                    checkReservedKeywordUsage(renderer, lines);
                    ParenthesesErrors(renderer,lines);
                    CheckSpellingErrors(lines,  Error5);
                    getUndefindvars(lines);
                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

                    if (!Errors.empty()){
                        boxRGBA(renderer, 144, 400, 800, 600, 100, 100, 100, 255);
                        UploadTxt(renderer, "Error: ", TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 15), {255, 0, 0, 255}, 150, 400);
                        int c = 0;
                        for (auto i : Errors) {
                            UploadTxt(renderer, i, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 12), {255, 0, 0, 255}, 150, 415 + c++*10);
                        }
                        SDL_RenderPresent(renderer);
                    }else{
                        boxRGBA(renderer, 144, 400, 800, 600, 100, 100, 100, 255);
                        UploadTxt(renderer, "No Error has been founded! Every thing is OK; compile succesfully!", TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 18), {0, 255, 0, 255}, 150, 450);
                        SDL_RenderPresent(renderer);
                    }



                } else if (e.button.x > 670 && e.button.x < 700 && e.button.y > 3 && e.button.y < 33 ) {
                    Errors.clear();
//                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
//                    checkUnclosedQuotes(renderer, lines);
//                    checkReservedKeywordUsage(renderer, lines);
//                    ParenthesesErrors(renderer,lines);
//                    CheckSpellingErrors(lines,  Error5);
                    libcommandcheker(renderer, lines);
                    if ((libincludecheker("bits/stdc++.h", lines) && Errors.empty()) || (Error.empty() && Errors.empty())) {
                        // If there's an error, choose rendering based on DARK_LIGHT flag
                        if (DARK_LIGHT) {
                            SDL_SetRenderTarget(renderer, bg);
                            MakingBg1(renderer);
                            textColor = {0, 0, 0, 255};
                            SDL_SetRenderTarget(renderer, nullptr);
//                            SDL_RenderPresent(renderer);
                        } else {
                            SDL_SetRenderTarget(renderer, bg);
                            MakingBg2(renderer);
                            textColor = {255, 255, 255, 255};
                            SDL_SetRenderTarget(renderer, nullptr);
                            SDL_RenderPresent(renderer);
                        }

                        // Render the page with the appropriate error message
                        RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                        compiler(lines);
                    } else  {
                        // If no errors, render the error box and message
                        if (!Error.empty())
                             Errors.insert(Errors.begin(),Error);

                        boxRGBA(renderer, 144, 400, 800, 600, 100, 100, 100, 255);
                        UploadTxt(renderer, "Error: Incuding libraries!", TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 19), {255, 0, 0, 255}, 150, 400);
                        int c = 0;
                        for (auto i : Errors) {
                            UploadTxt(renderer, i, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 17), {255, 0, 0, 255}, 180, 415 + c++*10);
                        }
                        SDL_RenderPresent(renderer);

//                        if(!(libcommandcheker(renderer, lines) || libincludecheker("bits/stdc++.h", lines)))
//                            UploadTxt(renderer, Error, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16), {255, 0, 0, 255}, 150, 560);
//                        if(!checkUnclosedQuotes(renderer, lines))
//                            UploadTxt(renderer, Error2, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16), {255, 0, 0, 255}, 150, 538);
//                        if(!checkReservedKeywordUsage(renderer, lines))
//                            UploadTxt(renderer, Error3, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16), {255, 0, 0, 255}, 150, 516);
//                        if(!ParenthesesErrors(renderer,lines))
//                            UploadTxt(renderer, Error4, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16), {255, 0, 0, 255}, 150, 494);
//                        if(!CheckSpellingErrors(lines,  Error5))
//                            UploadTxt(renderer, Error5, TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16), {255, 0, 0, 255}, 150, 472);
//                        cout << Error5;
//                        cout << 999;
//                        SDL_RenderPresent(renderer);
                        //
                        std::string code;
                        for (const auto& line : lines) {
                            code += line + "\n"; // اضافه کردن هر خط به رشته اصلی
                        }

                        // فراخوانی تابع استخراج متغیرها
                        variables = extractVariables(code);

                    }


                } else if (e.button.x > 8 && e.button.x < 144 ) {
                    if (toggleFile){
                        if (e.button.y > 70 && e.button.y < filenames.size()*30 + 70){
                            int x = (e.button.y - 70)/30;
                            CourentFile = filenames[x];
                            if (!CourentFile.empty()){
                                //CourentFile.insert(CourentFile.begin(), ' ');
                                ifstream in3("..\\" + FolderName + "\\" + CourentFile );
                                lines.clear();
                                if (in3.good()){
                                    string gb;
                                    while (!in3.eof()){
                                        getline(in3,gb);
                                        lines.push_back(gb);
                                    }
                                } else{
                                    cerr << "error opening previos file...2 ";
                                }
                                in3.close();
                            }
                            undochar = "";
                            undopos.clear();
                            undobool.clear();
                            UndoPointer = -1;
                            undoredocurrentcheker = false;
                            currentLine = 1;
                            ensureLastLineVisible(currentTime,scrollOffset,SCREEN_HEIGHT,LINE_HEIGHT, lines.size());
                            RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

                        }

                    }

                }  else if (e.button.x > 1 && e.button.x < 144 && e.button.y > 20 && e.button.y < 70 ) {
                    toggleFile = (toggleFile) ? false : true;
                    cout << e.button.x << " " << e.button.y << endl;
                    if (DARK_LIGHT){
                        SDL_SetRenderTarget(renderer,bg);

                        MakingBg1(renderer);
                        textColor = {0,0,0,255};

                        SDL_SetRenderTarget(renderer, nullptr);
                        //SDL_RenderCopy(renderer,bg, nullptr, nullptr);


                        SDL_RenderPresent(renderer);
                    } else{


                        SDL_SetRenderTarget(renderer,bg);

                        MakingBg2(renderer);
                        textColor = {255,255,255,255};

                        SDL_SetRenderTarget(renderer, nullptr);
                        //SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                        SDL_RenderPresent(renderer);




                    }

                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

                }else if(e.button.x > 707 && e.button.x < 737 && e.button.y > 5 && e.button.y < 35)
                {
                    if (DARK_LIGHT){
                        DARK_LIGHT = false;



                        SDL_SetRenderTarget(renderer,bg);

                        MakingBg2(renderer);
                        textColor = {255,255,255,255};

                        SDL_SetRenderTarget(renderer, nullptr);
                        //SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                        SDL_RenderPresent(renderer);
                    } else{
                        DARK_LIGHT = true;


                        SDL_SetRenderTarget(renderer,bg);

                        MakingBg1(renderer);
                        textColor = {0,0,0,255};

                        SDL_SetRenderTarget(renderer, nullptr);
                        //SDL_RenderCopy(renderer,bg, nullptr, nullptr);


                        SDL_RenderPresent(renderer);
                    }

                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

                }else if (e.button.x > 740 && e.button.x < 765 && e.button.y > 8 && e.button.y < 33) {
                    //openning command box...
                    bool exit = false;
                    string filename ;
                    string explainaition = "click a button to start command...";
                    SDL_SetRenderTarget(renderer,bg);

                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                    UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                    SDL_SetRenderTarget(renderer, nullptr);
                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                    SDL_RenderPresent(renderer);
                    // changing the way of the loop...
                    while (!exit) {
                        while (SDL_PollEvent(&e) != 0) {
                            if (e.type == SDL_MOUSEBUTTONDOWN) {
                                if (e.button.x > 620 && e.button.x < 650 && e.button.y > 100 && e.button.y < 130) {
                                    exit = true;
                                }else if (e.button.x > 350 && e.button.x < 450 && e.button.y > 140 && e.button.y < 200) {
                                    //new file
                                    bool exit1 = false;
                                    explainaition = "Enter filename : ";
                                    SDL_SetRenderTarget(renderer,bg);

                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                    UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                                    SDL_SetRenderTarget(renderer, nullptr);
                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                    SDL_RenderPresent(renderer);
                                    while (!exit1) {
                                        while (SDL_PollEvent(&e) != 0) {
                                            if (e.type == SDL_TEXTINPUT) {
                                                if (e.text.text) {
                                                    filename += e.text.text;
                                                    SDL_SetRenderTarget(renderer,bg);

                                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                                    UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                                                    SDL_SetRenderTarget(renderer, nullptr);
                                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                                    SDL_RenderPresent(renderer);
                                                }
                                                //refreshing command box...
                                                SDL_SetRenderTarget(renderer,bg);

                                                UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                                UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                                UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                                                SDL_SetRenderTarget(renderer, nullptr);
                                                SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                                SDL_RenderPresent(renderer);
                                            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                                                if (e.button.x > 620 && e.button.x < 650 && e.button.y > 100 &&
                                                    e.button.y < 130) {
                                                    exit1 = true;
                                                    exit = true;
                                                } else if (e.button.x > 150 && e.button.x < 650 && e.button.y > 400 && e.button.y < 500) {
                                                    if (filename != " ") {
                                                        NewFile(filename);
                                                        lines = {""};
                                                        ifstream in1("..\\" + FolderName + "\\" + CourentFile + ".cpp");
                                                        if (in1.good()) {
                                                            string gb;
                                                            while (!in1.eof()) {
                                                                getline(in1, gb);
                                                                lines.push_back(gb);
                                                            }
                                                        } else {
                                                            cerr << "error opening previos file...3 ";
                                                        }
                                                        in1.close();
                                                    }
                                                    CourentFile+=".cpp";

                                                    ofstream on2("mem\\prevousFILE.txt");
                                                    if (on2.good()) {
                                                        on2 << CourentFile;
                                                    } else {
                                                        cerr << "error finding previos file... ";
                                                    }
                                                    on2.close();

                                                    ofstream on3("mem\\allfilesNAME.txt", ios::app);
                                                    if (on3.good()) {
                                                        on3 << CourentFile << endl;
                                                    } else {
                                                        cerr << "error saving new file name... ";
                                                    }
                                                    on3.close();
                                                    exit1 = true;
                                                    exit = true;
                                                }
                                            }
                                            else if (e.type == SDL_KEYDOWN) {
                                                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                                                    int s = filename.length();
                                                    filename.erase(s - 1, 1);
                                                    SDL_SetRenderTarget(renderer,bg);

                                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                                    UploadTxt(renderer,filename,font,{0,0,0,255},180,325);
                                                    SDL_SetRenderTarget(renderer, nullptr);
                                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                                    SDL_RenderPresent(renderer);
                                                }
                                            }
                                        }
                                    }
                                }else if (e.button.x > 150 && e.button.x < 350 && e.button.y > 140 && e.button.y < 270) {
                                    //save file
                                    ofstream out("..\\" + FolderName + "\\" + CourentFile );
                                    if(out.good()){
                                        for (auto i : lines) {
                                            out << i << endl ;
                                        }
                                    }
                                    out.close();
                                    explainaition = "The Courrent file saved succesfully!";
                                    SDL_SetRenderTarget(renderer,bg);

                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                    SDL_SetRenderTarget(renderer, nullptr);
                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                    SDL_RenderPresent(renderer);

                                }else if (e.button.x > 470 && e.button.x < 550 && e.button.y > 140 && e.button.y < 270) {
                                    //enter directory address
                                    bool exit2 = false;
                                    explainaition = "Enter favorit directory address : ";
                                    SDL_SetRenderTarget(renderer,bg);

                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                    UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                                    SDL_SetRenderTarget(renderer, nullptr);
                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                    SDL_RenderPresent(renderer);
                                    while (!exit2) {
                                        while (SDL_PollEvent(&e) != 0) {
                                            if (e.type == SDL_TEXTINPUT) {
                                                if (e.text.text) {
                                                    filename += e.text.text;
                                                    SDL_SetRenderTarget(renderer,bg);

                                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                                    UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                                                    SDL_SetRenderTarget(renderer, nullptr);
                                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                                    SDL_RenderPresent(renderer);
                                                }
                                            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                                                if (e.button.x > 620 && e.button.x < 650 && e.button.y > 100 && e.button.y < 130) {
                                                    exit2 = true;
                                                    exit = true;
                                                }else if (e.button.x > 150 && e.button.x < 650 && e.button.y > 400 && e.button.y < 500) {
                                                    //save file
                                                    ofstream out(filename);
                                                    if(out.good()){
                                                        for (auto i : lines) {
                                                            out << i << endl ;
                                                        }
                                                    }else{
                                                        cout << "Error!";
                                                    }
                                                    out.close();
                                                    exit2 = true;
                                                    exit = true;
                                                }
                                            }else if (e.type == SDL_KEYDOWN) {
                                                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                                                    int s = filename.length();
                                                    filename.erase(s - 1, 1);
                                                    SDL_SetRenderTarget(renderer,bg);

                                                    UploadSvg(renderer,"CMD.jpg",150,100,500,400);
                                                    UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
                                                    UploadTxt(renderer,filename + " ",font,{0,0,0,255},180,325);
                                                    SDL_SetRenderTarget(renderer, nullptr);
                                                    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                                                    SDL_RenderPresent(renderer);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

//                        SDL_SetRenderTarget(renderer,bg);
//
//                        UploadSvg(renderer,"CMD.jpg",150,100,500,400);
//                        UploadTxt(renderer,explainaition,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20),{0,0,0,255},180,300);
//                        UploadTxt(renderer,filename,font,{0,0,0,255},180,325);
//                        SDL_SetRenderTarget(renderer, nullptr);
//                        SDL_RenderCopy(renderer,bg, nullptr, nullptr);
//
//                        SDL_RenderPresent(renderer);

                    }
                    if (DARK_LIGHT){
                        SDL_SetRenderTarget(renderer,bg);

                        MakingBg1(renderer);
                        textColor = {0,0,0,255};

                        SDL_SetRenderTarget(renderer, nullptr);
                        //SDL_RenderCopy(renderer,bg, nullptr, nullptr);


                        SDL_RenderPresent(renderer);
                    } else{


                        SDL_SetRenderTarget(renderer,bg);

                        MakingBg2(renderer);
                        textColor = {255,255,255,255};

                        SDL_SetRenderTarget(renderer, nullptr);
                        //SDL_RenderCopy(renderer,bg, nullptr, nullptr);

                        SDL_RenderPresent(renderer);




                    }

                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    if ((selectionStartLine <= selectionEndLine) && selection ) {
                        // Handle the case where the selection spans multiple lines
                        if (selectionStartLine == selectionEndLine) {
                            // If the selection is within a single line
                            lines[selectionStartLine].erase(selectionStartPos, selectionEndPos - selectionStartPos);
                        } else {
                            // Remove the text from the first line (selectionStartLine)
                            lines[selectionStartLine].erase(selectionStartPos);

                            // Remove the text from the last line (selectionEndLine)
                            lines[selectionEndLine].erase(0, selectionEndPos);

                            // Remove the entire lines in between (if any)
                            for (int line = selectionStartLine + 1; line < selectionEndLine; ++line) {
                                lines[line].clear();  // Clear the whole line
                            }
                        }

                        // After removing the text, reset the selection and cursor
                        selectionStartLine = 0;
                        selectionEndLine = 0;
                        selectionStartPos = 0;
                        selectionEndPos = 0;
                        cursorPos = selectionStartPos;

                        // Optionally, re-render the updated content
                        RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                        selection = false;
                    }
                    // Ensure cursorPos is within the valid range
                    if (cursorPos > 1 && cursorPos <= lines[currentLine].size()) {
                        // Remove character before cursor
                        if (undoredocurrentcheker){
                            undochar = "";
                            cout << undochar.length();
                            undopos.clear();
                            undobool.clear();
                            UndoPointer = 0;
                            undoredocurrentcheker = false;
                        }else
                            UndoPointer++;
                        undobool.push_back(false);
                        undochar +=  lines[currentLine][cursorPos - 1];
                        undopos.push_back({currentLine,cursorPos - 1});
                        lines[currentLine].erase(cursorPos - 1, 1);

                        cursorPos--;
                    } else if (currentLine > 0) {
                        // Merge with previous line
                        cursorPos = lines[currentLine - 1].size();
                        lines[currentLine - 1] += lines[currentLine].substr(1, lines[currentLine].size());
                        lines.erase(lines.begin() + currentLine);
                        currentLine--;
                    }

                    // Ensure there's always at least one line
                    if (lines.empty()) {
                        lines.push_back("");
                        currentLine = 0;
                        cursorPos = 0;
                    }

                    RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    if (cursorPos <= lines[currentLine].size()) {
                        std::string remainder = lines[currentLine].substr(cursorPos);
                        lines[currentLine] = lines[currentLine].substr(0, cursorPos);
                        lines.insert(lines.begin() + currentLine + 1, remainder);
                        currentLine++;
                        cursorPos = 0;
                        ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                        RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                    }
                } else if (e.key.keysym.sym == SDLK_TAB) {
                    if (selectionStartLine <= selectionEndLine && selection) {
                        // Iterate through the lines in the selection
                        for (int line = selectionStartLine; line <= selectionEndLine; ++line) {
                            // Insert 4 spaces at the beginning of each selected line
                            lines[line].insert(0, "    ");  // "    " is 4 spaces
                        }

                        // Optionally, reset the selection after the operation
                        selectionStartLine = 0;
                        selectionEndLine = 0;
                        selectionStartPos = 0;
                        selectionEndPos = 0;
                        cursorPos = selectionStartPos;

                        // Re-render the page to show the changes
                        RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                        selection = false;
                    }
//                    ofstream out("..\\kapa.txt" , ios::app);
//                    if (out.good())
//                        out << "helow mohammad ali!...all of these hard days will be finished soon... \n";
//                        else
//                            cerr << "error opening file...";






                    // Add spaces for tab
                    if (matchcomplet){
                        lines[currentLine].erase(cursorPos - patern.length() ,patern.length());
                        cursorPos -= patern.length();
                        lines[currentLine].insert(cursorPos,mp[AutoCompletChheker(patern)] );
                        cursorPos += mp[AutoCompletChheker(patern)].length();
                        matchcomplet = false;
                    }else{
                        // Add spaces for tab
                        lines[currentLine].insert(cursorPos, "    ");
                        cursorPos += 4;
                    }
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    // Move cursor left
                    if (cursorPos > 1) {
                        cursorPos--;
                    } else if (currentLine > 0) {
                        currentLine--;
                        cursorPos = lines[currentLine].size();
                    }
                } else if (e.key.keysym.sym == SDLK_RIGHT) {
                    // Move cursor right
                    if (cursorPos < lines[currentLine].size()) {
                        cursorPos++;
                    } else if (currentLine < lines.size() - 1) {
                        currentLine++;
                        cursorPos = 0;
                    }
                } else if (e.key.keysym.sym == SDLK_UP) {
                    // Move cursor up
                    if (currentLine > 0) {
                        currentLine--;
                        cursorPos = std::min(cursorPos, (int) lines[currentLine].size());

                        ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                    }
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    if (currentLine < lines.size() - 1) {
                        currentLine++;
                        cursorPos = std::min(cursorPos, (int) lines[currentLine].size());
                        ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                    }
                }

                RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);

            }
            else if (e.type == SDL_TEXTINPUT) {
                if (selectionStartLine <= selectionEndLine && selection) {
                    // First, remove the selected text
                    for (int line = selectionStartLine; line <= selectionEndLine; ++line) {
                        // If it's the first line in the selection
                        if (line == selectionStartLine) {
                            lines[line].erase(selectionStartPos, selectionEndPos - selectionStartPos);
                        }
                            // If it's the last line in the selection
                        else if (line == selectionEndLine) {
                            lines[line].erase(0, selectionEndPos);
                        }
                            // If it's a line in between, just clear the line
                        else {
                            lines[line].clear();
                        }
                    }


                    // Re-render the page to show the updated content
                    RenderPage(renderer, bg, scrollOffset, lines, LINE_HEIGHT, font, currentLine, cursorPos, textColor);
                    selection = false;
                }

                matchcomplet = false;
                bool braket = false;
                if (e.text.text) {
                    lines[currentLine].insert(cursorPos, e.text.text);
                    //auto completing brakets , prantes and ...
                    if (e.text.text[0] == '{'){
                        lines[currentLine].insert(cursorPos + 1, "}");
                        braket = true;
                    }else if (e.text.text[0] == '('){
                        lines[currentLine].insert(cursorPos + 1, ")");
                        braket = true;
                    } else if (e.text.text[0] == '['){
                        lines[currentLine].insert(cursorPos + 1, "]");
                        braket = true;
                    }else if (e.text.text[0] == '"'){
                        lines[currentLine].insert(cursorPos + 1, "\"");
                        braket = true;
                    }else if (e.text.text[0] == '\''){
                        lines[currentLine].insert(cursorPos + 1, "'");
                        braket = true;
                    }


                    if (!braket){
                        // auto completing Keywords...

                        int i = cursorPos;
                        patern = "";

                        while(lines[currentLine][i] != ' ' && lines[currentLine][i] != '\n' && lines[currentLine][i] != '{' && lines[currentLine][i] != '[' && lines[currentLine][i] != '(' && lines[currentLine][i] != '}' && lines[currentLine][i] != ']' && lines[currentLine][i] != ')' )
                            patern += lines[currentLine][i--];
                        string paternreverse = "";
                        for (int j = patern.length() - 1; j >= 0; --j) {
                            paternreverse += patern[j];
                        }
                        patern = paternreverse;
                        cout << patern << endl;
                        if (!patern.empty())
                            if (AutoCompletChheker(patern) != "no match!")
                                matchcomplet = true;
                    }

                    //adding current changes into memory for undo/redo...
                    if (undoredocurrentcheker){
                        undochar = "";
                        cout << undochar.length();
                        undopos.clear();
                        undobool.clear();
                        UndoPointer = 0;
                        undoredocurrentcheker = false;
                    }else
                        UndoPointer++;
                    undobool.push_back(true);
                    undochar += e.text.text;
                    undopos.push_back({currentLine,cursorPos});

                    cursorPos += strlen(e.text.text);
                    ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());


                }

                RenderPage(renderer,bg,scrollOffset,lines,LINE_HEIGHT,font,currentLine,cursorPos,textColor);
                if (matchcomplet){
                    boxRGBA(renderer, 180 + cursorPos*10,70 + LINE_HEIGHT*currentLine,180 + cursorPos*10 + 200, 70 + LINE_HEIGHT*(currentLine + 1),100,100,100,220);
                    UploadTxt(renderer,mp[AutoCompletChheker(patern)],TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16),{255,0,81,255},180 + cursorPos*10 + 10,70 + LINE_HEIGHT*currentLine + 5);
                    SDL_RenderPresent(renderer);
                }

            }
        }



        // Clear screen
        // SDL_SetRenderTarget(renderer, bg);
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderClear(renderer);
        // MakingBg2(renderer);
        // SDL_SetRenderTarget(renderer, nullptr);




        SDL_RenderPresent(renderer);

    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Extracted Variables:\n";
    for (const auto& var : variables) {
        std::cout << var << std::endl;
    }
    return 0;

}
void OpeningURL(string url){
    string path = "cmd /c start chrome " + url;
    system(path.c_str());
}

void LoadingAnimation (SDL_Renderer* r){
    UploadSvg(r,"MY_codeLOGO.jpg",0,0,800,600);
    SDL_RenderPresent(r);
    SDL_Delay(3000);
    for (int i = 0; i < 4; ++i) {
        UploadSvg(r,"MY_codeLOGO.jpg",0,0,800,600);
        boxRGBA(r,50,550,50 + 175*(i+1),560,248,81,52,255);
        UploadTxt(r, to_string((i+1)*25) + "% loading...",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 24),{248,81,52,200},50,520);
        SDL_RenderPresent(r);
        SDL_Delay(3000);
    }

}

void ensureLastLineVisible(int currentLine, int &scrollOffset, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines) {
    int cursorY = currentLine * LINE_HEIGHT - scrollOffset;
    if (cursorY < 0) {
        // Scroll up
        scrollOffset = currentLine * LINE_HEIGHT;
    } else if (cursorY + LINE_HEIGHT > SCREEN_HEIGHT) {
        // Scroll down
        scrollOffset = (currentLine + 1) * LINE_HEIGHT - SCREEN_HEIGHT;
    }

    // Ensure last line is always visible
    int contentHeight = totalLines * LINE_HEIGHT;
    if (contentHeight > SCREEN_HEIGHT) {
        scrollOffset = std::min(scrollOffset, contentHeight - SCREEN_HEIGHT);
    } else {
        scrollOffset = -40; // No scrolling needed if content fits
    }
}

void UploadSvg (SDL_Renderer * r, const char *path , int x , int y , int w , int h){

    SDL_Surface* imageSurface = IMG_Load(path);
    if (!imageSurface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();

    }
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(r, imageSurface);
    if (!imageTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(imageSurface);
        IMG_Quit();
        SDL_Quit();

    }
    SDL_FreeSurface(imageSurface);
    SDL_Rect destRect = {x, y, w, h}; // x, y, width, height
    SDL_RenderCopy(r, imageTexture, nullptr, &destRect);
    SDL_DestroyTexture(imageTexture);
}

void MakingBg1 (SDL_Renderer* r){
    //bg color
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderClear(r);
    //bg navigation bar...
    for (int i = 0; i < 800 ; ++i) {
        aalineRGBA( r, i, 0, i, 40, 0.32*i, 60, 80, 255);

    }

    //bg nav buttons...
    UploadSvg (r, "timeslight.png" , 768 , 4 , 33 , 33);
    UploadSvg (r, "settings.png" , 740 , 8 , 25 , 25);
    UploadSvg (r, "darklightmode.png" , 707 , 5 , 30 , 30);
    UploadSvg (r, "runcode.png" , 677 , 8 , 25 , 25);
    UploadSvg(r,"DebugeAndCompile.png",642,10,25,25);
    UploadSvg (r, "Redo.png" , 600 , 8 , 25 , 25);
    UploadSvg (r, "Undo.png" , 570 , 8 , 25 , 25);

    //navigation main MY_code transparency logo...
    UploadSvg(r,"Transparentlogo.png",15,-5,55,55);
    //github logo for refrence...
    UploadSvg(r,"github-mark.png",80,4,33,33);
    UploadTxt(r,"Courent file :",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16),{160,160,160,255},138,15);
    //roundedBoxRGBA(renderer,760,8,788,33,2,252,170,11,255);
    //aalineRGBA(renderer,)



    //bg left nav ...
    for (int i = 0; i < 145 ; ++i) {
        aalineRGBA( r, i, 41, i, 600, 82, 82, 82, 255);

    }

    //boxRGBA(r,0,41,144,69,82,150,20,255);
    for (int i = 0; i < 145; ++i) {
        aalineRGBA(r,i,41,i,69,100 + i,14,62,255);
    }
    UploadTxt(r,FolderName,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 15),{255,255,255,255},21,48);


    if (toggleFile)
        UploadSvg(r,"Chevrondown.png",1,50,16,10);
    else
        UploadSvg(r,"Chevronright.png",4,47,10,16);
    if(Mode_File && toggleFile){

        if (filenames.empty()){
            string command = "dir /b ..\\" + FolderName + "\\*.cpp" +  " > file_list.txt";
            system(command.c_str());
            ifstream in ("file_list.txt");
            string s;
            while (!in.eof()){
                in >> s;
                filenames.push_back(s);
            }
            in.close();
        }

        //making the boxes of every file to open...
        for (int i = 0; i < filenames.size(); ++i) {
            //boxRGBA(r,8,70 + i*30,144,70 + (i+1)*30,200,150,20,255);
            for (int j = 8; j < 145; ++j) {
                aalineRGBA(r,j,70 + i*30,j,70 + (i+1)*30,255,30 + j/1.5,62,255);
            }
            string fn;
            if (filenames[i].length() < 14){
                fn = filenames[i];
            }else{
                fn = filenames[i].substr(0,13) + "...";
            }
            UploadTxt(r,fn ,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 14),{255,255,255,255},10,75 + i*30);
            // aalineRGBA(r,8,70 + (i+1)*30 ,145,70 + (i+1)*30,255,255,255,255);
        }
        //drawing a bar under every file box...
        for (int i = 0; i <  filenames.size(); ++i) {
            aalineRGBA(r,8,70 + (i+1)*30 ,143,70 + (i+1)*30,255,255,255,255);
        }



    }
}

void MakingBg2 (SDL_Renderer* r){
    //bg color
    SDL_SetRenderDrawColor(r, 23, 23, 23, 255);
    SDL_RenderClear(r);
    //bg navigation bar...
    for (int i = 0; i < 800 ; ++i) {
        aalineRGBA( r, i, 0, i, 40, 0.32*i, 60, 80, 255);

    }
    //bg nav buttons...
    UploadSvg (r, "timeslight.png" , 768 , 4 , 33 , 33);
    UploadSvg (r, "settings.png" , 740 , 8 , 25 , 25);
    UploadSvg (r, "darklightmode.png" , 707 , 5 , 30 , 30);
    UploadSvg (r, "runcode.png" , 677 , 8 , 25 , 25);
    UploadSvg(r,"DebugeAndCompile.png",642,10,25,25);
    UploadSvg (r, "Redo.png" , 600 , 8 , 25 , 25);
    UploadSvg (r, "Undo.png" , 570 , 8 , 25 , 25);
    //
    //navigation main MY_code transparency logo...
    UploadSvg(r,"Transparentlogo.png",15,-5,55,55);
    //github logo for refrence...
    UploadSvg(r,"github-mark.png",80,4,33,33);
    UploadTxt(r,"Courent file :",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 16),{160,160,160,255},138,15);
    //roundedBoxRGBA(renderer,760,8,788,33,2,252,170,11,255);
    //aalineRGBA(renderer,)



    //bg left nav ...
    for (int i = 0; i < 145 ; ++i) {
        aalineRGBA( r, i, 41, i, 600, 82, 82, 82, 255);

    }

    //boxRGBA(r,0,41,144,69,82,150,20,255);
    for (int i = 0; i < 145; ++i) {
        aalineRGBA(r,i,41,i,69,100 + i,14,62,255);
    }
    UploadTxt(r,FolderName,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 15),{255,255,255,255},21,48);


    if (toggleFile)
        UploadSvg(r,"Chevrondown.png",1,50,16,10);
    else
        UploadSvg(r,"Chevronright.png",4,47,10,16);
    if(Mode_File && toggleFile){

        if (filenames.empty()){
            string command = "dir /b ..\\" + FolderName + "\\*.cpp" +  " > file_list.txt";
            system(command.c_str());
            ifstream in ("file_list.txt");
            string s;
            while (!in.eof()){
                in >> s;
                filenames.push_back(s);
            }
            in.close();
        }

        //making the boxes of every file to open...
        for (int i = 0; i < filenames.size(); ++i) {
            //boxRGBA(r,8,70 + i*30,144,70 + (i+1)*30,200,150,20,255);
            for (int j = 8; j < 145; ++j) {
                aalineRGBA(r,j,70 + i*30,j,70 + (i+1)*30,255,30 + j/1.5,62,255);
            }
            string fn;
            if (filenames[i].length() < 14){
                fn = filenames[i];
            }else{
                fn = filenames[i].substr(0,13) + "...";
            }
            UploadTxt(r,fn ,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 14),{255,255,255,255},10,75 + i*30);
            // aalineRGBA(r,8,70 + (i+1)*30 ,145,70 + (i+1)*30,255,255,255,255);
        }
        //drawing a bar under every file box...
        for (int i = 0; i <  filenames.size(); ++i) {
            aalineRGBA(r,8,70 + (i+1)*30 ,143,70 + (i+1)*30,255,255,255,255);
        }



    }
}

void UploadTxt (SDL_Renderer * r, string text , TTF_Font* f , SDL_Color c, int x , int y ){

    SDL_Surface *textSurface = TTF_RenderText_Blended(f, text.c_str(), c);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(r, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect renderQuad = {x, y, textWidth, textHeight};

    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(r, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void compiler (vector <string> codes){
    ofstream out("..\\Exacutable.cpp");
    if(out.good()){
        for (auto i : codes) {
            out << i << endl ;
        }
    }
    out.close();
    if (system("g++ ..\\Exacutable.cpp -o ..\\Exacutable.exe") != 0) {
        std::cerr << "Compilation failed." << std::endl;

    }

    // Step 3: Run the compiled program
    if (run_in_another_window("..\\Exacutable.exe") != 0) {
        std::cerr << "Execution failed." << std::endl;

    }
}

int run_in_another_window(const std::string &filename)
{
#ifdef _WIN32 // Windows
    std::string runCommand = "start cmd /c \"" + filename + " & pause\"";
#elif __linux__ // Linux
    std::string runCommand = "x-terminal-emulator -e \"" + filename + "; read -n 1 -s -r -p 'Press any key to continue...'\"";
#elif __APPLE__ // macOS
    std::string runCommand = "osascript -e 'tell application \"Terminal\" to do script \"" + filename + "\"'";
#else
    std::cerr << "Unsupported operating system." << std::endl;
    return;
#endif

    std::system(runCommand.c_str());

    return 0;
}

void NewFile(string filename) {
    ofstream out("..\\" + FolderName + "\\" + filename + ".cpp");
    if (out.good()) {
        out << "#include <iostream>\n int main() { \n std::cout << " <<"\"Hellwo MY_code User!\"" << "; \n return 0 ; \n}" << endl;
    }
    CourentFile = filename;
}

void RenderPage(SDL_Renderer * renderer, SDL_Texture * bg, int scrollOffset, vector<string>lines , int LINE_HEIGHT,TTF_Font * font,int currentLine,int cursorPos, SDL_Color textColor){
    //refreshing pages...
    SDL_RenderCopy(renderer,bg, nullptr, nullptr);

    UploadTxt (renderer, CourentFile  ,  TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20) , {160,160,160,255}, 265 , 12 );


    // Render text

    int y = -scrollOffset + 10; // Start rendering based on the scroll offset
//        if (y < 50)
//            y = 40;
    for (size_t i = 0; i < lines.size(); ++i) {

        bool ign = false;
        if (y < 50){
            y += LINE_HEIGHT;
            ign = true;
        } else if (y + LINE_HEIGHT > 0 && y < SCREEN_HEIGHT) { // Render only visible lines
            if (lines[i].empty()) {
                lines[i] = " "; // Show cursor on the current line
            }

            //drawing line Numbers in the left side of them...
            UploadTxt(renderer, to_string(i+1),TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 12),textColor,146,y + 4);
            // Get the list of words with their respective colors
            std::vector<std::pair<std::string, SDL_Color>> words = splitLineIntoWords(lines[i],inMultilineComment);

            int wordX = 170; // Start drawing words from this X position
            for (const auto &wordPair : words) {
                const std::string &word = wordPair.first;
                SDL_Color wordColor = wordPair.second; // Get the color for the word

                SDL_Surface *textSurface = TTF_RenderText_Blended(font, word.c_str(), wordColor);
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                int textWidth = textSurface->w;
                int textHeight = textSurface->h;
                SDL_Rect renderQuad = {wordX, y, textWidth, textHeight};

                SDL_FreeSurface(textSurface);

                SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
                SDL_DestroyTexture(textTexture);

                wordX += textWidth; // Move X to the right for the next word
            }


            // Render cursor if this is the current line
            if (i == currentLine) {
                int cursorX = 0;
                if (cursorPos > 0) {
                    TTF_SizeText(font, lines[i].substr(0, cursorPos).c_str(), &cursorX, nullptr);
                }
                cursorX += 170 ; // Add padding for the left margin
                SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
                SDL_RenderDrawLine(renderer, cursorX, y, cursorX, y + LINE_HEIGHT);
                if (CntG){
                    boxRGBA(renderer,146,y,800,y + LINE_HEIGHT,255,164,25,150);
                    CntG = false;
                }
            }
        }
        if(!ign)
            y += LINE_HEIGHT; // Move to the next line
        // Update screen
        // SDL_RenderPresent(renderer);
    }
}

std::vector<std::pair<std::string, SDL_Color>> splitLineIntoWords(const std::string& line, bool &inMultilineComment) {
    std::vector<std::pair<std::string, SDL_Color>> words;

    // Flags to indicate if we are inside a string, character literal, or multiline comment
    bool inString = false;  // Flag for strings (double quotes)
    bool inCharLiteral = false;  // Flag for character literals (single quotes)
    bool inSingleLineComment = false;  // Flag for single-line comments
    std::string currentWord;

    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];

        // Check if we encounter the start of a multiline comment (/*)
        if (!inMultilineComment && i + 1 < line.length() && line[i] == '/' && line[i + 1] == '*') {
            inMultilineComment = true;
            // Add the comment token (/*) itself
            words.push_back({"/*", comment});
            i++;  // Skip the next character as it's part of the comment start
            continue;
        }

        // Check if we encounter the end of a multiline comment (*/)
        if (inMultilineComment && i + 1 < line.length() && line[i] == '*' && line[i + 1] == '/') {
            inMultilineComment = false;
            // Add the comment token (*/) itself
            words.push_back({"*/", comment});
            i++;  // Skip the next character as it's part of the comment end
            continue;
        }

        // Check if we encounter a single-line comment (//)
        if (!inMultilineComment && !inSingleLineComment && i + 1 < line.length() && line[i] == '/' && line[i + 1] == '/') {
            // Start of a single-line comment
            inSingleLineComment = true;
            words.push_back({"//", comment});
            i++;  // Skip the next character as it's part of the comment start
            continue;
        }

        if (inSingleLineComment) {
            // Everything after // is part of the single-line comment, color it as a comment
            words.push_back({std::string(1, ch), comment});
        }
        else if (inMultilineComment) {
            // Everything inside the multiline comment should be colored as a comment
            words.push_back({std::string(1, ch), comment});
        }
        else if (inString) {
            // Handle characters inside a string (double quotes)
            currentWord += ch;
            if (ch == '"') {
                // End of string, push it and reset the string flag
                if(DARK_LIGHT)
                    words.push_back({currentWord, dark_green}); // Color the entire string green
                else
                    words.push_back({currentWord, light_green}); // Color the entire string green
                currentWord.clear();
                inString = false;
            }
        }
        else if (inCharLiteral) {
            // Handle characters inside a character literal (single quotes)
            currentWord += ch;
            if (ch == '\'') {
                // End of character literal, push it and reset the char literal flag
                if(DARK_LIGHT)
                    words.push_back({currentWord, dark_green}); // Color the entire char literal green
                else
                    words.push_back({currentWord, light_green});
                currentWord.clear();
                inCharLiteral = false;
            }
        }
        else if (ch == '"') {
            // Start of string (double quotes), push the opening quote and change the flag
            if (!currentWord.empty()) {
                words.push_back({currentWord, getKeywordColor(currentWord)});
                currentWord.clear();
            }
            inString = true;
            currentWord += ch;  // Add the opening quote
        }
        else if (ch == '\'') {
            // Start of character literal (single quote), push the opening quote and change the flag
            if (!currentWord.empty()) {
                words.push_back({currentWord, getKeywordColor(currentWord)});
                currentWord.clear();
            }
            inCharLiteral = true;
            currentWord += ch;  // Add the opening single quote
        }
        else if (ch == ' ') {
            // If it's a space, add the current word to the list and reset
            if (!currentWord.empty()) {
                words.push_back({currentWord, getKeywordColor(currentWord)});
                currentWord.clear();
            }
            if (DARK_LIGHT)
                words.push_back({std::string(1, ch), dark_green});
            else
                words.push_back({std::string(1, ch), SPACE_COLOR});  // Add the space
        }
        else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ',' || ch == '.' || ch == ';') {
            // If the character is a punctuation mark, handle it
            if (!currentWord.empty()) {
                words.push_back({currentWord, getKeywordColor(currentWord)});
                currentWord.clear();
            }
            if (DARK_LIGHT)
                words.push_back({std::string(1, ch), golden});
            else
                words.push_back({std::string(1, ch), light_black});
        }
        else if (ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '0') {
            // Handle numbers
            if (!currentWord.empty()) {
                words.push_back({currentWord, getKeywordColor(currentWord)});
                currentWord.clear();
            }
            if (DARK_LIGHT)
                words.push_back({std::string(1, ch), purple});
            else
                words.push_back({std::string(1, ch), orange});
        }
        else {
            // Otherwise, build the current word (alphanumeric or other)
            currentWord += ch;
        }
    }

    // If there's any leftover word, push it to the result
    if (!currentWord.empty()) {
        words.push_back({currentWord, getKeywordColor(currentWord)});
    }

    UnableVariableNaming(words);
    SemicolonErrors(words);
    //OperandErrors(words);

    return words;

}

SDL_Color getKeywordColor(const std::string &word) {
    if (word == "int" || word == "float" || word == "double" || word == "long" || word == "char" || word == "void" || word == "string" || word == "short" || word == "struct" || word == "class" || word == "using") {
        if(DARK_LIGHT){
            return green_blue; // Type keyword color
        }
        else{
            return red;}
    }
    else if (word == "if" || word == "else" || word == "while" || word == "return" || word == "switch" || word == "for" || word == "namespace") {
        if(DARK_LIGHT){
            return dark_blue; // Type keyword color
        }
        else{
            return light_purple;}
    } else if (word == "{" || word == "}" || word == "(" || word == ")" || word == "[" || word == "]" || word == "std" ) {
        if(DARK_LIGHT){
            return golden; // Type keyword color
        }
        else{
            return light_black;}
    }else if (word == "+" || word == "-" || word == "=" || word == "*" || word == "/" || word == "&" || word == "%") {
        if(DARK_LIGHT){
            return cherry; // Type keyword color
        }
        else{
            return dark_orange2;}
    } else {
        if(DARK_LIGHT){
            return BLACK; // Type keyword color
        }
        else{
            return WHITE;}
    }
}


bool libincludecheker (string s ,  vector<string> a){
    if (s == "bits/stdc++.h")
        s = "bits/stdc\\+\\+\\.h";
    regex r("\\s*#include\\s*<\\s*" + s + "\\s*>") , cut("\\s*int\\s*main\\s*(\\s*)");
    int i =0;
    while (!regex_search(a[i],cut)){
        if (regex_search(a[i],r))
        { cout << "included " << s << endl;
            return true;}
        i++;
    }
    return false;
}

bool libcommandcheker(SDL_Renderer *r , vector<string> a){
    bool iostream = false , cmath = false , action = false;
    regex io(R"((\s*cout\s*)|(\s*cin\s*)|(\s*endl\s*)|(\s*getline\s*))"),
            cm(R"(\s*sqrt\s*\(\s*.*\s*\)\s*|\s*pow\s*\(\s*.*\s*,\s*.*\s*\)\s*|\s*sin\s*\(\s*.*\s*\)\s*|\s*cos\s*\(\s*.*\s*\)\s*|\s*tan\s*\(\s*.*\s*\)\s*|\s*abs\s*\(\s*.*\s*\)\s*|\s*exp\s*\(\s*.*\s*\)\s*|\s*log\s*\(\s*.*\s*\)\s*|\s*log10\s*\(\s*.*\s*\)\s*|\s*floor\s*\(\s*.*\s*\)\s*|\s*ceil\s*\(\s*.*\s*\)\s*)");
    int c =0 ;
    for (auto i : a) {
        if (regex_search(i,cm)){
            cmath = true;
            cout << "contains cmath in line " << c + 1 << endl;

        }

        if (regex_search(i,io)){
            iostream = true;
            cout << "contains iostream in line " << c + 1 << endl;

        }
        c++;

    }

    Error = "Not included ";

    if(!iostream && ! cmath){
        Error = "";
        return true;
    }


    if (iostream && !libincludecheker("iostream" , a)){
        action = true;
        Error += "<iostream> ";
    }


    if (cmath && !libincludecheker("cmath" , a)){
        action = true;
        Error += "<cmath> ";
    }

    if (action){
        Error += "! Include theme before compilation...";
        return false;
        // ErrorHandler(r,Error);

    } else{
        Error = "";
        return true;
    }



}

void ErrorHandler(SDL_Renderer* r,string s){
    boxRGBA(r,144,500,800,600,100,100,100,255);
    UploadTxt(r,"Error : ",TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 24) ,{255,0,0,255},150,510);
    UploadTxt(r,s,TTF_OpenFont("C:\\Windows\\Fonts\\consola.ttf", 20) ,{255,0,0,255},150,560);
    SDL_RenderPresent(r);


}

string AutoCompletChheker(string s){
    if (s != "" && s != " "){
        string k[24];
        //loops and conditinal statemants...
        k[0] = "if";
        k[1] = "else";
        k[2] = "while";
        k[3] = "switch";
        //variables...
        k[4] = "int";
        k[5] = "float";
        k[6] = "double";
        k[7] = "string";
        k[8] = "long long";
        k[9] = "char";
        k[10] = "main";
        //functions...
        k[11] = "cout";
        k[12] = "cin";
        k[13] = "endl";
        k[14] = "pow";
        k[15] = "abs";
        k[16] = "min";
        k[17] = "max";
        k[18] = "printf";
        k[19] = "scanf";
        k[20] = "strlen";
        k[21] = "for";
        k[22] = "#include";
        k[23] = "using";
        regex r("^"+s);
        for (auto i : k) {
            if (regex_search(i,r)){
                return i;
            }

        }
    }


    return "no match!";
}

void UndoRedo(vector<string> &a){
    if (UndoPointer>=0)
        if (undobool[UndoPointer]){
            a[undopos[UndoPointer][0]].erase(undopos[UndoPointer][1],1);
            currentLine = undopos[UndoPointer][0];
            cursorPos = undopos[UndoPointer][1];
            undobool[UndoPointer] = false;

        }else{
            a[undopos[UndoPointer][0]].insert(a[undopos[UndoPointer][0]].begin()+undopos[UndoPointer][1],undochar[UndoPointer]);
            currentLine = undopos[UndoPointer][0];
            cursorPos = undopos[UndoPointer][1] + 1;
            undobool[UndoPointer] = true;
        }
}

bool hasUnclosedQuotes(const string& line) {
    bool inQuotes = false;
    for (char ch : line) {
        if (ch == '"' || ch == '\'') {
            inQuotes = !inQuotes; // Toggle the inQuotes flag
        }
    }
    return inQuotes; // Returns true if there's an unclosed quote
}
// Function to check all lines for unclosed double quotes
bool checkUnclosedQuotes(SDL_Renderer *r, vector<string> lines) {
    // Initialize the error string
    bool hasError = false;

    for (size_t i = 0; i < lines.size(); ++i) {
        if (hasUnclosedQuotes(lines[i])) {
            hasError = true;
            Errors.push_back("Unclosed double/single quotes in line " + to_string(i + 1));

        }
    }

    if (hasError) {
        return false; // Errors found
    } else {
        return true; // No errors found
    }
}

void UnableVariableNaming(vector<pair<string, SDL_Color>> &words){
    string bg = words[0].first;
    int i = 0;
    bool varcheker = false;
    bool magnitudcheker = false;
    while (bg == " " && i + 1 < words.size()){

        bg = words[++i].first;
    }
    if (bg == "int" || bg == "float" || bg == "string" || bg == "double" || bg == "long long" || bg == "char"){
        varcheker = true;
        for (int j = i + 1; j < words.size(); ++j) {
            if (words[j].first == ";"){
                varcheker = false;
                magnitudcheker = false;
                continue;
            }else if (words[j].first == "=" && varcheker) {
                magnitudcheker = true;
                continue;
            }else if (words[j].first == " "){
                continue;
            }else if (words[j].first == "," && varcheker) {
                magnitudcheker = false;
                continue;
            }else{
                if (magnitudcheker){
                    magnitudcheker = false;
                    continue;
                }else if ((words[j].first == "int" || words[j].first == "float" || words[j].first == "string" || words[j].first == "double" || words[j].first == "long long" || words[j].first == "char") && varcheker){
                    words[j].second = Error_Color;
                    Errors.push_back("Reseved KeyWord : " + words[j].first +  " used in variable naming" );
                }else if((words[j].first == "int" || words[j].first == "float" || words[j].first == "string" || words[j].first == "double" || words[j].first == "long long" || words[j].first == "char") && !varcheker){
                    varcheker = true;
                    continue;
                }else if (varcheker){
                    regex  rp("([.;\\\\:'!#$%&*])|^[0-9]");
                    if(regex_search(words[j].first,rp)){
                        words[j].second = Error_Color;
                        Errors.push_back("Unable variable naming in word : " + words[j].first);
                    }
                }
            }
        }
    }


}

void SemicolonErrors(vector<pair<string, SDL_Color>> &words){
    int i = words.size() - 1;
    string bg = words[i].first;
    bool semicoloncheker = false;
    //bool magnitudcheker = false;
    while (bg == " " && i > 0){
        bg = words[--i].first;
    }
    if (bg == ";" || bg == "}" || bg == "{")
        return ;
    else{
        for (auto j : words) {
            string line = j.first;
            if (line.find("if") != string::npos || line.find("if (") != string::npos ||
                line.find("for") != string::npos || line.find("for (") != string::npos ||
                line.find("while") != string::npos || line.find("while (") != string::npos ||
                line.find("else") != string::npos ||
                line.find("switch") != string::npos || line.find("switch (") != string::npos ||
                line.find("class") != string::npos || line.find("struct") != string::npos ||
                line.find("namespace") != string::npos || line.find("template") != string::npos ||
                line.find("main") != string::npos || line.find("namespace ") != string::npos ||
                line.find("//") != string::npos || line.find("namespace ") != string::npos ||
                line.find("/*") != string::npos || line.find("namespace ") != string::npos ||
                line.find("*/") != string::npos || line.find("namespace ") != string::npos ||
                line.find("#") != string::npos) { // Preprocessor directives
                semicoloncheker = true;
            }
        }
        if (!semicoloncheker){
            if (words[i].first != " " && words[i].first != ""){
                words[i].second = Error_Color;
                Errors.push_back("Semicolon Error after word : " + words[i].first);
            }


        }
    }
}


void OperandErrors(vector<pair<string, SDL_Color>> &words) {
    int i = words.size() - 1;
    string bg = words[i].first;
    bool operatorChecker = false;

    // Skip any empty space or irrelevant tokens at the end
    while (bg == " " && i > 0) {
        bg = words[--i].first;
    }

    // If the last word is a known valid operator or part of a valid expression, return early
    if (bg == ";" || bg == "}" || bg == "{") {
        return;
    } else {
        // Define valid operators
        vector<string> validOperators = { "++", "--", "+", "-", "*", "/", "%", "=", "+=", "-=", "*=", "/=", "%=" };

        // Regex to match invalid operand combinations like "+++", "====", etc.
        regex invalidOperandRegex("([\\+\\-\\*\\/\\%=]{2,})");  // Matches sequences like "++", "+++" or "===="

        // Iterate over words and check if they are valid operands or operators
        for (auto& j : words) {
            string line = j.first;

            // Skip preprocessor directives like #include <iostream> or #define
            if (line.find("#include") != string::npos || line.find("#define") != string::npos ||
                line.find("#ifdef") != string::npos || line.find("#endif") != string::npos) {
                continue;  // Skip these lines
            }

            // Check for invalid operands using regex
            if (regex_search(line, invalidOperandRegex)) {
                // If an invalid operand sequence is found, change color to error color
                j.second = Error_Color;  // Mark it with error color
                Errors.push_back("Operand Error founded : " + line);
                operatorChecker = true;
            }

            // Check if the word is a valid operator or operand (not invalid ones)
            bool validOperand = false;
            for (const auto& validOp : validOperators) {
                if (line.find(validOp) != string::npos) {
                    validOperand = true;
                    break;
                }
            }

            // If the word isn't a valid operator or operand, mark it with error color
            if (!validOperand && !regex_search(line, invalidOperandRegex)) {
                j.second = Error_Color;  // Mark it with error color
                Errors.push_back("Operand Error founded : " + line);
                operatorChecker = true;
            }
        }

    }
}

vector<string> reservedKeywords = {
        "int", "float", "double", "char", "if", "else", "while", "for", "return", "void", "switch", "case", "break"
        , "int;", "float;", "double;", "char;", "if;", "else;", "while;", "for;", "return;", "void;", "switch;", "case;", "break;" ,"char","string","main" ,"using","namespace","struct","class"
};

bool hasReservedKeywordAfterType(const string& line) {
    istringstream stream(line);
    string word;
    bool foundType = false;

    while (stream >> word) {
        // اگر یک نوع داده پیدا کردیم (مثلاً int، float، double)
        if (word == "int" || word == "float" || word == "double" || word == "char" || word == "long" || word == "string" || word == "void" ) {
            foundType = true;
        } else if (foundType) {
            // پس از نوع داده، کلمه‌ای به عنوان متغیر باید بیاید
            // اگر این کلمه یک کلمه رزرو شده باشد، اشتباه است
            for (const string& keyword : reservedKeywords) {
                if (word == keyword) {
                    return true; // کلمه رزرو شده به عنوان متغیر استفاده شده
                }
            }
            foundType = false; // اگر کلمه بعدی متغیر بود، دیگر ادامه ندهیم
        }
    }
    return false;
}

bool checkReservedKeywordUsage(SDL_Renderer *r, vector<string> lines) {
    // ارورهای یافت شده در رشته Error3
    bool hasError = false;

    for (size_t i = 0; i < lines.size(); ++i) {
        if (hasReservedKeywordAfterType(lines[i])) {
            hasError = true;
            //Error3 += "Incorrect variable in line " + to_string(i + 1);
            Errors.push_back("Incorrect variable in line " + to_string(i + 1));

        }
    }

    if (hasError) {
        // چاپ ارور یا افزودن به Error3
        cout << Error3; // یا اینکه می‌توانید Error3 را در یک متغیر جهانی یا خارجی ذخیره کنید.
        return false; // خطاها یافت شدند
    } else {
        return true; // هیچ خطایی پیدا نشد
    }
}

bool ParenthesesErrors(SDL_Renderer* r, vector<string> lines) {
    stack<char> parenthesesStack;  // برای ذخیره پرانتزها، براکت‌ها و کروشه‌ها
    bool hasError = false;

    // بررسی تمامی خطوط کد
    for (size_t i = 0; i < lines.size(); ++i) {
        string line = lines[i];

        // بررسی هر کاراکتر در خط
        for (size_t j = 0; j < line.size(); ++j) {
            char c = line[j];

            // اگر کاراکتر باز است (پرانتز، براکت یا کروشه باز)
            if (c == '(' || c == '{' || c == '[') {
                parenthesesStack.push(c);
            }
                // اگر کاراکتر بسته است (پرانتز، براکت یا کروشه بسته)
            else if (c == ')' || c == '}' || c == ']') {
                // اگر پشته خالی باشد، یعنی هیچ پرانتز یا براکت باز برای این بسته یافت نمی‌شود
                if (parenthesesStack.empty()) {
                    hasError = true;
                    // اضافه کردن پیام خطا به Error3 با شماره خط
                    //Error4 += "Error in line " + to_string(i + 1) + ": Unmatched closing parenthesis/bracket/brace.\n";
                    Errors.push_back("Error in line " + to_string(i + 1) + ": Unmatched closing parenthesis/bracket/brace.");
                    break;
                }

                // بررسی تطابق با کاراکتر باز مناسب
                char top = parenthesesStack.top();
                if ((c == ')' && top == '(') || (c == '}' && top == '{') || (c == ']' && top == '[')) {
                    parenthesesStack.pop();  // تطابق پیدا کرد، بنابراین آن را از پشته خارج می‌کنیم
                } else {
                    // اگر تطابق نداشت
                    hasError = true;
                    // اضافه کردن پیام خطا به Error3 با شماره خط
                    //Error4 += "Error in line " + to_string(i + 1) + ": Mismatched parenthesis/bracket/brace.\n";
                    Errors.push_back("Error in line " + to_string(i + 1) + ": Mismatched parenthesis/bracket/brace.");
                    break;
                }
            }
        }

        if (hasError) {
            break;
        }
    }

    // بررسی در انتهای کد
    if (!parenthesesStack.empty()) {
        // اگر پشته خالی نیست، یعنی پرانتز باز بدون بسته شدن داریم
        hasError = true;
        // اضافه کردن پیام خطا به Error3
        //Error4 += "Error: Unmatched opening parenthesis/bracket/brace.\n";
        Errors.emplace_back("Error: Unmatched opening parenthesis/bracket/brace.");
    }

    // تابع بولی: اگر خطا یافت شد، مقدار false برمی‌گرداند
    return !hasError;  // اگر خطا نباشد، true برگشت می‌دهیم، در غیر این صورت false
}

std::vector<std::string> validKeywords = {"while", "for", "if", "else", "int","float","double" ,"char","string","main" ,"using","namespace","struct","class", "return", "void", "break", "continue", "switch", "case", "default"};

// تابع برای محاسبه فاصله ویرایشی (Levenshtein distance) بین دو کلمه
int LevenshteinDistance(const std::string& s1, const std::string& s2) {
    size_t len1 = s1.size();
    size_t len2 = s2.size();
    std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i) {
        for (size_t j = 0; j <= len2; ++j) {
            if (i == 0) {
                dp[i][j] = j;  // اگر رشته اول خالی است، باید تمام کلمات دوم را اضافه کنیم
            } else if (j == 0) {
                dp[i][j] = i;  // اگر رشته دوم خالی است، باید تمام کلمات اول را حذف کنیم
            } else {
                dp[i][j] = std::min({dp[i-1][j] + 1, dp[i][j-1] + 1, dp[i-1][j-1] + (s1[i-1] != s2[j-1])});
            }
        }
    }

    return dp[len1][len2];
}

// تابع برای بررسی غلط‌های املایی در کلمات
bool CheckSpellingErrors(const std::vector<std::string>& lines, std::string& Error5) {
    bool hasError = false;
    Error5 = "";
    const int maxDistance = 2; // اگر فاصله ویرایشی کمتر از این مقدار باشد، به عنوان اشتباه املایی شبیه به کلمه معتبر در نظر می‌گیریم

    // بررسی تمامی خطوط کد
    for (size_t i = 0; i < lines.size(); ++i) {
        std::string line = lines[i];
        bool insideQuotes = false; // وضعیت داخل یا خارج کوتیشن
        size_t start = 0;

        // جدا کردن کلمات از خط
        for (size_t j = 0; j <= line.size(); ++j) {
            // اگر به کوتیشن رسیدیم، وضعیت داخل/خارج کوتیشن را تغییر دهید
            if (j < line.size() && line[j] == '"') {
                insideQuotes = !insideQuotes;
            }

            // اگر خارج از کوتیشن هستیم و به یک فاصله یا انتهای خط رسیدیم، کلمه را استخراج می‌کنیم
            if (!insideQuotes && (j == line.size() || !std::isalnum(line[j]))) {
                std::string word = line.substr(start, j - start);

                // بررسی اینکه آیا این کلمه در لیست کلمات کلیدی معتبر وجود ندارد
                if (!word.empty() && word.length() >= 2) { // فقط کلمات با طول ۲ یا بیشتر را بررسی کنید
                    bool isExactMatch = false;
                    bool isSimilar = false;
                    std::string suggestedCorrections;

                    // نادیده گرفتن اعداد خالص
                    bool isNumeric = std::all_of(word.begin(), word.end(), [](char c) { return std::isdigit(c); });
                    if (isNumeric) continue;

                    // بررسی تطابق دقیق
                    for (const std::string& validKeyword : validKeywords) {
                        if (word == validKeyword) {
                            isExactMatch = true;
                            break;
                        }
                    }

                    // بررسی تشابه
                    if (!isExactMatch) {
                        for (const std::string& validKeyword : validKeywords) {
                            if (LevenshteinDistance(word, validKeyword) <= maxDistance) {
                                isSimilar = true;
                                suggestedCorrections += validKeyword + " ";
                            }
                        }
                    }

                    // اگر کلمه شبیه به یکی از کلمات کلیدی بود، خطا گزارش دهید
                    if (isSimilar) {
                        hasError = true;
                        //Error5 += "Spelling error in line " + std::to_string(i + 1) + ":" + word + " is not a valid keyword.Did you mean " + suggestedCorrections + "?\n";
                        Errors.push_back("Spelling error in line " + to_string(i + 1) + ":" + word + " is not a valid keyword.Did you mean " + suggestedCorrections + "?");
                    }
                }

                // شروع کلمه جدید بعد از کاراکتر غیر الفبا/عدد
                start = j + 1;
            }
        }
    }

    return !hasError;  // اگر خطای املایی وجود نداشته باشد، true برگشت می‌دهیم
}

std::vector<std::string> extractVariables(const std::string& code) {
    std::vector<std::string> variables;
    std::regex variableRegex(R"((int|float|double|char|bool|string)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*(?:=\s*[^;]+)?\s*;)");

    auto words_begin = std::sregex_iterator(code.begin(), code.end(), variableRegex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string variableName = match[2].str();
        variables.push_back(variableName);
    }

    return variables;
}


//undefind variables
void getUndefindvars(vector<string> code){
    vector <string> definedvars;
    for (auto line : code){

        vector <string> words;
        string word;
        istringstream iss(line);
        while (iss >> word){
            words.push_back(word);
        }
        for (int i = 0; i < words.size() ; i++) {
            string tvar = words[i];
            if (tvar == "int" || tvar == "char" || tvar == "double" || tvar == "string" || tvar == "float" || tvar == "void" || tvar == "long")
            {
                if (words[i+1][words[i+1].length() - 1] == ';'){
                    definedvars.push_back(words[i+1].substr(0,words[i+1].length() - 1));
                } else{
                    definedvars.push_back(words[i+1]);
                }



            }
        }

    }

    for (auto line : code){
        string word;
        istringstream iss(line);
        while(iss >> word){
            if (word == "int" || word == "char" || word == "double" || word == "string" || word == "float" || word == "void"
                || word == "pow" || word == "sqrt" || word == "abs" || word == "min" || word == "max" || word == "exp"
                || word[word.length() - 1] == '(' || word[word.length() - 1] == ')' || word[word.length() - 1] == '{' || word[word.length() - 1] == '}' || word[word.length() - 1] == '>' || word[word.length() - 1] == '<' || word[word.length() - 1] == '"' || word[word.length() - 1] == '\'' || word[0] == '"' || word[0] == '\''|| word == ";" || word == ","
                || word == "}" || word == "{" || word == "[" || word == "]" || word == "(" || word == ")"
                || word == "main" || word == "#include" || word == "using" || word == "namespace" || word == "std;" || word == "struct" || word == "class"
                || word == "if" || word == "else" || word == "while" || word == "switch" || word == "break" || word == "continue" || word == "for"
                || word == "0" || word == "1" || word == "2" || word == "3" || word == "4" || word == "5" || word == "6" || word == "7" || word == "8" || word == "9"
                || word == "+" || word == "-" || word == "*" || word == "=" || word == "cout" || word == "std::cout" || word == "cin" || word == "std::cin" || word == "endl;" || word == "std::endl;"
                || word == "return" || word == "++" || word == "--" || word == "+" || word == "-" || word == "*" || word == "/" || word == "%" || word == "=" || word == "+=" || word == "-=" || word == "*=" || word == "/=" || word == "%="){
                continue;
            }else{
                if (word[word.length() - 1] == ';')
                    word = word.substr(0,word.length() - 1);
                int i = count(definedvars.begin(), definedvars.end(),word);
                if ( i == 0 ){
                    if (word.find('+') == string::npos && word.find('-') == string::npos && word.find('*') == string::npos && word.find('=') == string::npos && word.find('/') == string::npos && word.find('%') == string::npos)
                        Errors.push_back("Undefind variable reccognized as : " + word);
                    else
                        Errors.push_back("Strang Operand reccognized as : " + word);
                }
            }
        }




    }


}