#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,  playerTurn(true),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Get the QWidget that will hold the grid layout
    QWidget *gridContainer = ui->gridContainer; // Ensure this name matches the object name in your UI file

    // Create a new QGridLayout
    QGridLayout *gridLayout = new QGridLayout(gridContainer);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Loop to create 10x10 grid of labels
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            // Create a QLabel for each cell
            QLabel *label = new QLabel(" ", this);
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet("border: 1px solid black; padding: 5px;");
            QFont font = label->font();
            font.setPointSize(14); // Set the font size (change this value as needed)
            label->setFont(font);
            label->setFixedSize(30, 30);  // Adjust this to your desired label size

            // Set the size policy to ensure it respects the fixed size
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            labels[row][col] = label;
            // Add the label to the grid layout at position (row, col)
            gridLayout->addWidget(label, row, col);
        }
    }

    // Set the layout for the container widget
    gridContainer->setLayout(gridLayout);
    gridContainer->setMinimumSize(350, 350); // Set the minimum size of the grid container (10 labels * 100px)
    gridContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Ensure the container expands if needed
    ss = displayBoard(board);
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col)
        {
            // Get the label at (row, col)
            string a;
            a += ss[row][col];
            labels[row][col]->setText(QString::fromStdString(a));            }
    }
    ui->Turnlabel->setText("Your Turn");
}
vector<vector<Piece*>> board = initializeBoard();

MainWindow::~MainWindow()
{
    delete ui;
}
const int BOARD_SIZE =8;

vector<vector<Piece*>> initializeBoard() {
    vector<vector<Piece*>> board(BOARD_SIZE, vector<Piece*>(BOARD_SIZE, nullptr));

    for (int i = 0; i < BOARD_SIZE; i++) {
        board[1][i] = new Piece('P', false);  // Black pawns
        board[6][i] = new Piece('P', true);   // White pawns
    }

    board[0][0] = new Piece('R', false); board[0][7] = new Piece('R', false);
    board[7][0] = new Piece('R', true);  board[7][7] = new Piece('R', true);
    board[0][1] = new Piece('N', false); board[0][6] = new Piece('N', false);
    board[7][1] = new Piece('N', true);  board[7][6] = new Piece('N', true);
    board[0][2] = new Piece('B', false); board[0][5] = new Piece('B', false);
    board[7][2] = new Piece('B', true);  board[7][5] = new Piece('B', true);
    board[0][3] = new Piece('Q', false); board[0][4] = new Piece('K', false);
    board[7][3] = new Piece('Q', true);  board[7][4] = new Piece('K', true);

    return board;
}



vector<vector<char>> displayBoard(const vector<vector<Piece*>>& board) {
    vector<vector<char>> board_chars(10, vector<char>(10,'*'));
    for(int i =0; i <10; ++i)
    {
        if(i <=7)
        {
            board_chars[i+1][0] = 8-i+48;
            board_chars[i+1][9] = 8-i+48;
        }
        for(int j =1; j <9; ++j)
        {
            char c ='A';
            if(i == 0 || i == 9)
            {
                board_chars[i][j] = c + j-1;
            }
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == nullptr) board_chars[i+1][j+1] = '.';
            else
            {
                char c = board[i][j]->getSymbol();
                board_chars[i+1][j+1] = ' ';
                board_chars[i+1][j+1] = c;
            }
        }
    }
    board_chars[0][0] = '\\' ;
    board_chars[9][9] = '\\' ;
    board_chars[0][9] = '/' ;
    board_chars[9][0] = '/' ;
    return board_chars;
}
pair<int, int> parseMove(const string& pos) {
    return { 8 - (pos[1] - '0'), pos[0] - 'A' };
}
bool isValidPosition(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}
vector<pair<int, int>> getValidMoves(const vector<vector<Piece*>>& board, int row, int col) {
    vector<pair<int, int>> moves;
    Piece* piece = board[row][col];
    if (!piece) return moves;

    int direction = piece->isWhite ? -1 : 1;

    if (piece->type == 'P') {  // Pawn moves
        // Forward movement
        if (isValidPosition(row + direction, col) && !board[row + direction][col]) {
            moves.push_back({row + direction, col});
            // Double move from starting position
            if ((piece->isWhite && row == 6) || (!piece->isWhite && row == 1)) {
                if (!board[row + 2 * direction][col]) {
                    moves.push_back({row + 2 * direction, col});
                }
            }
        }
        // Captures
        if (isValidPosition(row + direction, col + 1) && board[row + direction][col + 1] && board[row + direction][col + 1]->isWhite != piece->isWhite)
            moves.push_back({row + direction, col + 1});
        if (isValidPosition(row + direction, col - 1) && board[row + direction][col - 1] && board[row + direction ][col - 1]->isWhite != piece->isWhite)
            moves.push_back({row + direction, col - 1});
    }
    else if (piece->type == 'R') {  // Rook moves
        for (int d = -1; d <= 1; d += 2) {  // Two directions: up/down, left/right
            for (int i = 1; isValidPosition(row + i * d, col); i++) {
                int newRow = row + i * d;
                if (board[newRow][col]) {
                    if (board[newRow][col]->isWhite != piece->isWhite) moves.push_back({newRow, col});
                    break;
                }
                moves.push_back({newRow, col});
            }
            for (int i = 1; isValidPosition(row, col + i * d); i++) {
                int newCol = col + i * d;
                if (board[row][newCol]) {
                    if (board[row][newCol]->isWhite != piece->isWhite) moves.push_back({row, newCol});
                    break;
                }
                moves.push_back({row, newCol});
            }
        }
    }
    else if (piece->type == 'B') {  // Bishop moves
        for (int dr = -1; dr <= 1; dr += 2) {
            for (int dc = -1; dc <= 1; dc += 2) {
                for (int i = 1; isValidPosition(row + i * dr, col + i * dc); i++) {
                    int newRow = row + i * dr;
                    int newCol = col + i * dc;
                    if (board[newRow][newCol]) {
                        if (board[newRow][newCol]->isWhite != piece->isWhite) moves.push_back({newRow, newCol});
                        break;
                    }
                    moves.push_back({newRow, newCol});
                }
            }
        }
    }
    else if (piece->type == 'Q') {  // Queen moves
        for (int d = -1; d <= 1; d += 2) {
            for (int i = 1; isValidPosition(row + i * d, col); i++) {
                int newRow = row + i * d;
                if (board[newRow][col]) {
                    if (board[newRow][col]->isWhite != piece->isWhite) moves.push_back({newRow, col});
                    break;
                }
                moves.push_back({newRow, col});
            }
            for (int i = 1; isValidPosition(row, col + i * d); i++) {
                int newCol = col + i * d;
                if (board[row][newCol]) {
                    if (board[row][newCol]->isWhite != piece->isWhite) moves.push_back({row, newCol});
                    break;
                }
                moves.push_back({row, newCol});
            }
        }
        // Diagonal moves
        for (int dr = -1; dr <= 1; dr += 2) {
            for (int dc = -1; dc <= 1; dc += 2) {
                for (int i = 1; isValidPosition(row + i * dr, col + i * dc); i++) {
                    int newRow = row + i * dr;
                    int newCol = col + i * dc;
                    if (board[newRow][newCol]) {
                        if (board[newRow][newCol]->isWhite != piece->isWhite) moves.push_back({newRow, newCol});
                        break;
                    }
                    moves.push_back({newRow, newCol});
                }
            }
        }
    }
    else if (piece->type == 'N') {  // Knight moves
        vector<pair<int, int>> knightMoves = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2}, {1, 2}, {2, -1}, {2, 1}
        };
        for (auto move : knightMoves) {
            int newRow = row + move.first;
            int newCol = col + move.second;
            if (isValidPosition(newRow, newCol) &&
                (!board[newRow][newCol] || board[newRow][newCol]->isWhite != piece->isWhite)) {
                moves.push_back({newRow, newCol});
            }
        }
    }
    else if (piece->type == 'K') {  // King moves
        vector<pair<int, int>> kingMoves = {
            {-1, -1}, {- 1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
        };
        for (auto move : kingMoves) {
            int newRow = row + move.first;
            int newCol = col + move.second;
            if (isValidPosition(newRow, newCol) &&
                (!board[newRow][newCol] || board[newRow][newCol]->isWhite != piece->isWhite)) {
                moves.push_back({newRow, newCol});
            }
        }
    }

    return moves;
}
bool movePiece(vector<vector<Piece*>>& board, const string& from, const string& to) {
    pair<int, int> start = parseMove(from);
    pair<int, int> end = parseMove(to);

    Piece* piece = board[start.first][start.second];
    if (!piece || (board[end.first][end.second] && board[end.first][end.second]->isWhite == piece->isWhite)) {
        return false;  // Invalid move (either no piece or trying to capture own piece)
    }

    vector<pair<int, int>> validMoves = getValidMoves(board, start.first, start.second);
    if (find(validMoves.begin(), validMoves.end(), end) == validMoves.end()) {
        return false;  // Move is not valid
    }

    // Move the piece
    delete board[end.first][end.second];  // Capture the piece if there is one
    board[end.first][end.second] = piece;
    board[start.first][start.second] = nullptr;

    return true;
}bool isKingInCheck(const vector<vector<Piece*>>& board, bool isWhite) {
    // Find the position of the king
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->type == 'K' && piece->isWhite == isWhite) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    // Check if any opposing pieces can attack the king
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->isWhite != isWhite) {
                // Get the valid moves of the opponent's piece
                vector<pair<int, int>> moves = getValidMoves(board, i, j);
                for (const auto& move : moves) {
                    if (move.first == kingRow && move.second == kingCol) {
                        return true; // King is in check
                    }
                }
            }
        }
    }
    return false; // King is not in check
}
bool isCheckmate(vector<vector<Piece*>>& board, bool isWhite) {
    // If the king is not in check, it can't be in checkmate
    if (!isKingInCheck(board, isWhite)) {
        return false;
    }

    // Check if the current player has any valid move that can remove the check
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            Piece* piece = board[i][j];
            if (piece && piece->isWhite == isWhite) {
                vector<pair<int, int>> moves = getValidMoves(board, i, j);
                for (const auto& move : moves) {
                    // Try moving the piece to remove the check
                    vector<vector<Piece*>> boardCopy = board; // Create a copy of the board
                    movePiece(boardCopy, string(1, 'A' + j) + to_string(8 - i), string(1, 'A' + move.second) + to_string(8 - move.first));
                    if (!isKingInCheck(boardCopy, isWhite)) {
                        return false; // There is at least one move that removes the check
                    }
                }
            }
        }
    }

    return true; // No moves can remove the check, so it's checkmate
}
string systemMove(vector<vector<Piece*>>& board) {
    srand(time(0));
    vector<pair<pair<int, int>, pair<int, int>>> possibleMoves;
    vector<pair<pair<int, int>, pair<int, int>>> captureMoves;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            Piece* piece = board[row][col];
            if (piece && !piece->isWhite) {
                vector<pair<int, int>> moves = getValidMoves(board, row, col);
                for (const auto& move : moves) {
                    if (board[move.first][move.second] && board[move.first][move.second]->isWhite) {
                        captureMoves.push_back({{row, col}, move});
                    } else {
                        possibleMoves.push_back({{row, col}, move});
                    }
                }
            }
        }
    }

    if (!captureMoves.empty()) {
        auto move = captureMoves[rand() % captureMoves.size()];
        int fromRow = move.first.first, fromCol = move.first.second;
        int toRow = move.second.first, toCol = move.second.second;
        movePiece(board, string(1, char('A' + fromCol)) + to_string(8 - fromRow), string(1, char('A' + toCol)) + to_string(8 - toRow));
        char c ='A' + fromCol;
        char c2 = 8 - fromRow + 48;
        string s = "System captured a piece from ";
        s += c;
        s += c2;
        c = 'A' + toCol;
        c2 = 8 - toRow +48;
        s += " to ";
        s += c;
        s+=c2;
        s+=", Your turn now";
        return s;
    } else if (!possibleMoves.empty()) {
        auto move = possibleMoves[rand() % possibleMoves.size()];
        int fromRow = move.first.first, fromCol = move.first.second;
        int toRow = move.second.first, toCol = move.second.second;
        movePiece(board, string(1, char('A' + fromCol)) + to_string(8 - fromRow), string(1, char('A' + toCol)) + to_string(8 - toRow));
        char c ='A' + fromCol;
        char c2 = 8 - fromRow + 48;
        string s = "System moved from ";
        s += c;
        s += c2;
        c = 'A' + toCol;
        c2 = 8 - toRow +48;
        s += " to ";
        s += c;
        s+=c2;
        s+= ", Your turn now";
        return s;

    } else {
        string s ="System has no valid moves.";
        return s;
    }
    string s;
    return s;
}void playGame() {
}




void MainWindow::on_fromedit_textChanged(const QString &arg1)
{
    from = arg1.toStdString();
}


void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    to = arg1.toStdString();
}


void MainWindow::on_pushButton_2_clicked()
{
    string str;

    if (isKingInCheck(board, playerTurn)) {
        str += (playerTurn ? "Your king is in check!" : "System's king is in check!");
        ui->Turnlabel->setText(QString::fromStdString(str));
        if (isCheckmate(board, playerTurn)) {
            str += (playerTurn ? "You lose!" : "You win!") ;
            ui->Turnlabel->setText(QString::fromStdString(str));

            return;
        }
    }

    if (playerTurn) {
        if (!movePiece(board, from, to)) {
            str += "Invalid move. Try again.";
            ui->Turnlabel->setText(QString::fromStdString(str));
            return;
        }
    } else {
        str += "System is making a move...";
        ui->Turnlabel->setText(QString::fromStdString(str));
        str = systemMove(board);
    }
    playerTurn = !playerTurn;
    ss = displayBoard(board);
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col)
        {
            // Get the label at (row, col)
            string a;
            a += ss[row][col];
            labels[row][col]->setText(QString::fromStdString(a));            }
    }

    if (isKingInCheck(board, playerTurn)) {
        str += (playerTurn ? "Your king is in check!" : "System's king is in check!");
        ui->Turnlabel->setText(QString::fromStdString(str));
        if (isCheckmate(board, playerTurn)) {
            str += (playerTurn ? "You lose!" : "You win!") ;
            ui->Turnlabel->setText(QString::fromStdString(str));

            return;
        }
    }

    if (playerTurn) {
        if (!movePiece(board, from, to)) {
            str += "Invalid move. Try again.";
            ui->Turnlabel->setText(QString::fromStdString(str));
            return;
        }
    } else {
        str += "System is making a move...";
        ui->Turnlabel->setText(QString::fromStdString(str));
        str = systemMove(board);
        ui->Turnlabel->setText(QString::fromStdString(str));

    }
    playerTurn = !playerTurn;
    ss = displayBoard(board);
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col)
        {
            // Get the label at (row, col)
            string a;
            a += ss[row][col];
            labels[row][col]->setText(QString::fromStdString(a));            }
    }

}


void MainWindow::on_pushButton_clicked()
{
    board = initializeBoard();
    ss = displayBoard(board);
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col)
        {
            // Get the label at (row, col)
            string a;
            a += ss[row][col];
            labels[row][col]->setText(QString::fromStdString(a));            }
    }
    ui->Turnlabel->setText("Your Turn");

}

