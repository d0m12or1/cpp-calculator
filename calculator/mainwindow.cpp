#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    SetText("0");
    ui->l_memory->setText("");
    ui->l_formula->setText("");

    const char* digits[] = {"tb_zero","tb_one","tb_two","tb_three","tb_four","tb_five","tb_six","tb_seven","tb_eight","tb_nine"};
    for (auto name : digits) {
        QPushButton* btn = findChild<QPushButton*>(name);
        connect(btn, &QPushButton::clicked, this, &MainWindow::DigitClicked);
    }

    connect(ui->tb_comma, &QPushButton::clicked, this, &MainWindow::DotClicked);
    connect(ui->tb_negate, &QPushButton::clicked, this, &MainWindow::NegateClicked);
    connect(ui->tb_backspace, &QPushButton::clicked, this, &MainWindow::BackspaceClicked);

    connect(ui->tb_add, &QPushButton::clicked, this, &MainWindow::OperationClicked);
    connect(ui->tb_substract, &QPushButton::clicked, this, &MainWindow::OperationClicked);
    connect(ui->tb_multiplicate, &QPushButton::clicked, this, &MainWindow::OperationClicked);
    connect(ui->tb_divide, &QPushButton::clicked, this, &MainWindow::OperationClicked);
    connect(ui->tb_power, &QPushButton::clicked, this, &MainWindow::OperationClicked);

    connect(ui->tb_equal, &QPushButton::clicked, this, &MainWindow::EqualClicked);
    connect(ui->tb_reset, &QPushButton::clicked, this, &MainWindow::ResetClicked);

    connect(ui->tb_ms, &QPushButton::clicked, this, &MainWindow::MemorySaveClicked);
    connect(ui->tb_mc, &QPushButton::clicked, this, &MainWindow::MemoryClearClicked);
    connect(ui->tb_mr, &QPushButton::clicked, this, &MainWindow::MemoryLoadClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetText(const QString& text) {
    input_number_ = NormalizeNumber(text);
    ui->l_result->setText(input_number_);
    active_number_ = input_number_.toDouble();
}

void MainWindow::AddText(const QString& suffix) {
    input_number_ += suffix;
    ui->l_result->setText(input_number_);
    active_number_ = input_number_.toDouble();

    if (current_operation_ != Operation::NO_OPERATION) {
        ui->l_formula->setText(
            QString("%1 %2").arg(QString::number(calculator_.GetValue()), OpToString(current_operation_))
            );
    }
}

QString MainWindow::RemoveTrailingZeroes(const QString &text) {
    for (qsizetype i = 0; i < text.size(); ++i) {
        if (text[i] != '0') return text.mid(i);
    }
    return "";
}

QString MainWindow::NormalizeNumber(const QString &text) {
    if (text.isEmpty()) return "0";
    if (text.startsWith('.')) return NormalizeNumber("0" + text);
    if (text.startsWith('-')) return "-" + NormalizeNumber(text.mid(1));
    if (text.startsWith('0') && !text.startsWith("0.")) return NormalizeNumber(RemoveTrailingZeroes(text));
    return text;
}

void MainWindow::DigitClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    if (input_number_ == "0") input_number_ = "";
    AddText(btn->text());
}

void MainWindow::DotClicked() {
    if (!input_number_.contains('.')) AddText(".");
}

void MainWindow::NegateClicked() {
    if (input_number_.isEmpty())
        input_number_ = "0";

    if (input_number_.startsWith('-'))
        input_number_.remove(0, 1);
    else
        input_number_.prepend('-');

    SetText(input_number_);
    active_number_ = input_number_.toDouble();

    if (current_operation_ != Operation::NO_OPERATION)
        UpdateFormulaDisplay(active_number_);
}

void MainWindow::BackspaceClicked() {
    if (!input_number_.isEmpty()) {
        input_number_.chop(1);
        if (input_number_.isEmpty() || input_number_ == "-") SetText("0");
        else SetText(input_number_);
    }
}

QString MainWindow::OpToString(Operation op) {
    switch(op) {
    case Operation::NO_OPERATION: return "";
    case Operation::ADDITION: return "+";
    case Operation::SUBTRACTION: return "−";
    case Operation::MULTIPLICATION: return "×";
    case Operation::DIVISION: return "÷";
    case Operation::POWER: return "^";
    }
    return "";
}

void MainWindow::SetOperation(Operation op) {
    if (current_operation_ == Operation::NO_OPERATION) {
        calculator_.SetValue(active_number_);
    }

    current_operation_ = op;
    input_number_.clear();

    ui->l_formula->setText(
        QString("%1 %2").arg(QString::number(calculator_.GetValue()), OpToString(op))
        );
}

void MainWindow::OperationClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString text = btn->text();

    if (text == "+") SetOperation(Operation::ADDITION);
    else if (text == "−") SetOperation(Operation::SUBTRACTION);
    else if (text == "×") SetOperation(Operation::MULTIPLICATION);
    else if (text == "÷") SetOperation(Operation::DIVISION);
    else if (text == "xʸ") SetOperation(Operation::POWER);
}

void MainWindow::UpdateFormulaDisplay(Number right, bool with_equal) {
    QString formula = QString("%1 %2 %3").arg(QString::number(calculator_.GetValue()), OpToString(current_operation_), QString::number(right));
    if (with_equal) formula += " =";
    ui->l_formula->setText(formula);
}

void MainWindow::EqualClicked() {
    if (current_operation_ == Operation::NO_OPERATION) return;

    UpdateFormulaDisplay(active_number_, true);

    switch(current_operation_) {
    case Operation::ADDITION: calculator_.ApplyOperation("+", active_number_); break;
    case Operation::SUBTRACTION: calculator_.ApplyOperation("-", active_number_); break;
    case Operation::MULTIPLICATION: calculator_.ApplyOperation("*", active_number_); break;
    case Operation::DIVISION: calculator_.ApplyOperation("/", active_number_); break;
    case Operation::POWER: calculator_.ApplyOperation("**", active_number_); break;
    }

    SetText(QString::number(calculator_.GetValue()));
    input_number_.clear();
    current_operation_ = Operation::NO_OPERATION;
}

void MainWindow::ResetClicked() {
    current_operation_ = Operation::NO_OPERATION;
    ui->l_formula->clear();
    SetText("0");
}

void MainWindow::MemorySaveClicked() {
    memory_ = active_number_;
    has_memory_ = true;
    ui->l_memory->setText("M");
}

void MainWindow::MemoryLoadClicked() {
    if (!has_memory_) return;
    SetText(QString::number(memory_));
    input_number_.clear();
}

void MainWindow::MemoryClearClicked() {
    has_memory_ = false;
    ui->l_memory->clear();
}
