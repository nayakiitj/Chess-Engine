#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

extern const int BOARD_SIZE;

class Piece {
public:
    char type;
    bool isWhite;

    Piece(char t, bool color) : type(t), isWhite(color) {}
    char getSymbol() const { return isWhite ? toupper(type) : tolower(type); }
};
extern vector<vector<Piece*>> board;
using namespace std;
vector<vector<Piece*>> initializeBoard();
vector<vector<char>> displayBoard(const vector<vector<Piece*>>& board);
pair<int, int> parseMove(const string& pos);
bool isValidPosition(int row, int col);
vector<pair<int, int>> getValidMoves(const vector<vector<Piece*>>& board, int row, int col) ;
bool movePiece(vector<vector<Piece*>>& board, const string& from, const string& to) ;
bool isKingInCheck(const vector<vector<Piece*>>& board, bool isWhite);
bool isCheckmate(vector<vector<Piece*>>& board, bool isWhite);
string systemMove(vector<vector<Piece*>>& board);
void playGame();

#include <QMainWindow>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    vector<vector<char>> ss;
    bool playerTurn;
    string from, to;
private slots:

    void on_fromedit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel* labels[10][10];
};
#endif // MAINWINDOW_H
