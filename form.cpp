#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    gw = new GraphicsWidget;
    ledit = new QLineEdit;
    list = new QListWidget;
    ledit->setPlaceholderText("x^2");
    ui->verticalLayout->addWidget(gw);
    ui->verticalLayout->addWidget(ledit);
    ui->horizontalLayout->addWidget(list);

//    connect(ledit, &QLineEdit::returnPressed, this, &Form::returnPressed);
//    connect(this, &Form::update_func, gw->cvs, &canvas::update_func);
//    connect(list, &QListWidget::itemDoubleClicked, this, &Form::listItemDoubleClicked);
}

Form::~Form()
{
    delete ui;
}

//void Form::returnPressed() {
//    emit update_func(ledit->text());
//    auto* funcl = gw->cvs->get_func();
//    list->clear();
//    while(funcl != nullptr) {
//        list->addItem(QString::fromStdString(Function::get_string(funcl->get_func())));
//        funcl = funcl->next();
//    }
//}


//void Form::listItemDoubleClicked(QListWidgetItem *item) {
//    auto*& funcl = gw->cvs->func;
//    FunctionL::erase_by_textformat(funcl, item->text());
//    gw->cvs->update();
//    list->removeItemWidget(item);
//    delete item;
//}
