/*
 * This file is part of ACS_MODEL.
 *
 * ACS_MODEL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACS_MODEL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACS_MODEL. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TIMERAPP_H
#define TIMERAPP_H

//#include "qboxlayout.h"
#include<QBoxLayout>
#include<QMainWindow>
#include<QTabWidget>
#include<QLineEdit>
#include<QTableWidget>
#include<QLabel>
#include<QPushButton>
#include<QDebug>
#include<QTableWidgetItem>
#include<QHeaderView>

class TabbedApp : public QMainWindow
{
    Q_OBJECT

public:
    TabbedApp(QWidget * parent = nullptr) : QMainWindow(parent)
    {
        QTabWidget * p_tabs = new QTabWidget(this);

        setCentralWidget(p_tabs);

        QWidget * p_formTab = createFormTab();
        p_tabs->addTab(p_formTab, "Form");

        QWidget * p_tableTab = createTableTab();
        p_tabs->addTab(p_tableTab, "Table");

    }
private:
    QLineEdit * _p_nameEdit;
    QLineEdit * _p_ageEdit;
    QTableWidget * _p_table;

    QWidget * createFormTab()
    {
        QWidget * formTab = new QWidget;
        QVBoxLayout * p_layout = new QVBoxLayout(formTab);

        QHBoxLayout * p_nameLayout = new QHBoxLayout();
        QLabel * p_nameLabel = new QLabel("Name: ");
        _p_nameEdit = new QLineEdit(formTab);
        p_nameLayout->addWidget(p_nameLabel);
        p_nameLayout->addWidget(_p_nameEdit);

        QHBoxLayout * p_ageLayout = new QHBoxLayout();
        QLabel * p_ageLabel = new QLabel("Age: ");
        _p_ageEdit = new QLineEdit(formTab);
        p_ageLayout->addWidget(p_ageLabel);
        p_ageLayout->addWidget(_p_ageEdit);

        QHBoxLayout * p_buttonLayout = new QHBoxLayout();
        QPushButton * p_submitButton = new QPushButton("Add",formTab);
        QPushButton * p_cancelButton = new QPushButton("Cancel",formTab);
        p_buttonLayout->addWidget(p_submitButton);
        p_buttonLayout->addWidget(p_cancelButton);

        p_layout->addLayout(p_nameLayout);
        p_layout->addLayout(p_ageLayout);
        p_layout->addLayout(p_buttonLayout);

        connect(p_submitButton, &QPushButton::clicked, this, &TabbedApp::addDataToTable);
        connect(p_cancelButton, &QPushButton::clicked, this, &TabbedApp::clearFormData);

        return formTab;
    }
    QWidget * createTableTab()
    {
        QWidget * p_tableTab = new QWidget();
        QVBoxLayout * p_layout = new QVBoxLayout(p_tableTab);

        _p_table = new QTableWidget(0, 2, p_tableTab);
        _p_table->setHorizontalHeaderLabels({"Name", "Age"});
        _p_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        _p_table->horizontalHeader()->setSectionsClickable(true);

        p_layout->addWidget(_p_table);

        QPushButton * p_deleteButton = new QPushButton("Delete", p_tableTab);
        p_layout->addWidget(p_deleteButton);


        connect(p_deleteButton, &QPushButton::clicked, this, &TabbedApp::removeSelectedRow);

        return p_tableTab;
    }
private slots:
    void addDataToTable()
    {
        QString name = _p_nameEdit->text();
        QString age = _p_ageEdit->text();

        if ( name.isEmpty() || age.isEmpty() ) { qDebug() << "Field have to be not blanked"; return; }

        bool status;
        int ageValue = age.toInt(&status);
        if( !status || ageValue <= 0 ) { qDebug() << "Age have to be greater than zero"; emit clearFormData(); return; }

        int row = _p_table->rowCount();
        _p_table->insertRow(row);
        _p_table->setItem(row, 0, new QTableWidgetItem(name) );
        _p_table->setItem(row, 1, new QTableWidgetItem(age) );

        emit clearFormData();
    }
    void clearFormData()
    {
        _p_nameEdit->clear();
        _p_ageEdit->clear();
    }
    void removeSelectedRow()
    {
        int row = _p_table->currentRow();
        if (row != -1) _p_table->removeRow(row);
    }
};

#endif // TIMERAPP_H
