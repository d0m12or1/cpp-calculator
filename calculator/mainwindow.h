#pragma once

#include "calculator.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void DigitClicked();
    void DotClicked();
    void NegateClicked();
    void BackspaceClicked();
    void OperationClicked();
    void EqualClicked();
    void ResetClicked();
    void MemorySaveClicked();
    void MemoryLoadClicked();
    void MemoryClearClicked();

private:
    Ui::MainWindow* ui;

    Calculator calculator_;
    QString input_number_;
    Number active_number_ = 0.0;

    enum class Operation {
        NO_OPERATION,
        MULTIPLICATION,
        DIVISION,
        SUBTRACTION,
        ADDITION,
        POWER
    };

    Operation current_operation_ = Operation::NO_OPERATION;

    Number memory_ = 0;
    bool has_memory_ = false;

    void SetText(const QString& text);
    void AddText(const QString& suffix);
    QString NormalizeNumber(const QString& text);
    QString RemoveTrailingZeroes(const QString& text);
    void SetOperation(Operation op);
    QString OpToString(Operation op);
    void UpdateFormulaDisplay(Number right = 0, bool with_equal = false);
};
