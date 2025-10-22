#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->cmb_controller, &QComboBox::currentTextChanged,
            this, &MainWindow::OnControllerChanged);

    const std::vector<QString> digitNames = {
        "tb_zero","tb_one","tb_two","tb_three","tb_four",
        "tb_five","tb_six","tb_seven","tb_eight","tb_nine"
    };
    for (const auto &name : digitNames)
    {
        if (auto btn = findChild<QPushButton*>(name))
        {
            connect(btn, &QPushButton::clicked, this, &MainWindow::OnDigitButtonClicked);
        }
    }

    const std::vector<QString> opButtons = {
        "tb_add", "tb_substract", "tb_multiplicate", "tb_divide", "tb_power"
    };
    for (const auto& name : opButtons)
    {
        if (auto btn = findChild<QPushButton*>(name))
        {
            connect(btn, &QPushButton::clicked, this, &MainWindow::OnOperationButtonClicked);
        }
    }

    const std::vector<QString> ctrlButtons = {
        "tb_reset", "tb_mc", "tb_mr", "tb_ms", "tb_negate", "tb_backspace", "tb_extra", "tb_equal"
    };
    for (const auto& name : ctrlButtons)
    {
        if (auto btn = findChild<QPushButton*>(name))
        {
            connect(btn, &QPushButton::clicked, this, &MainWindow::OnControlButtonClicked);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetInputText(const std::string& text)
{
    ui->l_result->setStyleSheet("");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetErrorText(const std::string& text)
{
    ui->l_result->setStyleSheet("color: red;");
    ui->l_result->setText(QString::fromStdString(text));
}

void MainWindow::SetFormulaText(const std::string& text)
{
    ui->l_formula->setText(QString::fromStdString(text));
}

void MainWindow::SetMemText(const std::string& text)
{
    ui->l_memory->setText(QString::fromStdString(text));
}

void MainWindow::SetExtraKey(const std::optional<std::string>& key)
{
    if (key.has_value())
    {
        ui->tb_extra->setText(QString::fromStdString(key.value()));
        ui->tb_extra->show();
    }
    else
    {
        ui->tb_extra->hide();
    }
}

void MainWindow::SetDigitKeyCallback(std::function<void(int key)> cb)
{
    digit_cb_ = std::move(cb);
}

void MainWindow::SetProcessOperationKeyCallback(std::function<void(Operation key)> cb)
{
    operation_cb_ = std::move(cb);
}

void MainWindow::SetProcessControlKeyCallback(std::function<void(ControlKey key)> cb)
{
    control_cb_ = std::move(cb);
}

void MainWindow::SetControllerCallback(std::function<void(ControllerType controller)> cb)
{
    controller_cb_ = std::move(cb);
}

void MainWindow::OnDigitButtonClicked()
{
    if (!digit_cb_) return;

    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int key = btn->text().toInt();
    digit_cb_(key);
}

void MainWindow::OnOperationButtonClicked()
{
    if (!operation_cb_) return;

    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString obj = btn->objectName();
    Operation op;

    if (obj == "tb_add") op = Operation::ADDITION;
    else if (obj == "tb_substract") op = Operation::SUBTRACTION;
    else if (obj == "tb_multiplicate") op = Operation::MULTIPLICATION;
    else if (obj == "tb_divide") op = Operation::DIVISION;
    else if (obj == "tb_power") op = Operation::POWER;
    else return;

    operation_cb_(op);
}

void MainWindow::OnControlButtonClicked()
{
    if (!control_cb_) return;

    auto btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString obj = btn->objectName();
    ControlKey key;

    if (obj == "tb_equal") key = ControlKey::EQUALS;
    else if (obj == "tb_reset") key = ControlKey::CLEAR;
    else if (obj == "tb_ms") key = ControlKey::MEM_SAVE;
    else if (obj == "tb_mr") key = ControlKey::MEM_LOAD;
    else if (obj == "tb_mc") key = ControlKey::MEM_CLEAR;
    else if (obj == "tb_negate") key = ControlKey::PLUS_MINUS;
    else if (obj == "tb_backspace") key = ControlKey::BACKSPACE;
    else if (obj == "tb_extra") key = ControlKey::EXTRA_KEY;
    else return;

    control_cb_(key);
}

void MainWindow::OnControllerChanged(const QString& text)
{
    if (!controller_cb_) return;

    if (text == "uint8_t") controller_cb_(ControllerType::UINT8_T);
    else if (text == "int") controller_cb_(ControllerType::INT);
    else if (text == "int64_t") controller_cb_(ControllerType::INT64_T);
    else if (text == "size_t") controller_cb_(ControllerType::SIZE_T);
    else if (text == "double") controller_cb_(ControllerType::DOUBLE);
    else if (text == "float") controller_cb_(ControllerType::FLOAT);
    else if (text == "Rational") controller_cb_(ControllerType::RATIONAL);

    if (tb_extra) {
        if (text == "double" || text == "float") {
            tb_extra->setVisible(true);
            tb_extra->setText(".");
        }
        else if (text == "Rational") {
            tb_extra->setVisible(true);
            tb_extra->setText("/");
        }
        else {
            tb_extra->setVisible(false);
        }
    }
    QApplication::processEvents();
}
