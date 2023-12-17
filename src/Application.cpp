#include "Application.h"
#include "enycpt.h"
#include <QtCore/QtCore>
#include <string>
#include <fstream>

// 不加这个fstream没法用，不知道为什么
using namespace std;

#define u8str QString::fromUtf8

// 常量（感觉没什么意义，但是祖宗之法不可变也）
const int MIN_WIDTH = 800;  // 最小宽度
const int MAX_WIDTH = 800;  // 最大宽度
const int MIN_HEIGHT = 350;  // 最小高度
const int MAX_HEIGHT = 350;  // 最大高度
const QString TITLE = u8str("XOR加密");  // 标题

// 复制文件
void copyfile(string srcp,string detp) {
	fstream srcf = fstream(srcp,ios_base::binary|ios_base::in);
	fstream detf = fstream(detp,ios_base::binary|ios_base::out|ios_base::trunc);
	detf << srcf.rdbuf();
}

Application::Application() : QWidget() {
	this->setObjectName("Application");

	this->userChanged = false;

	this->setupUI();
}

Application::~Application() {

}

// 初始化UI
void Application::setupUI() {
	this->setWindowTitle(TITLE);
	this->setMinimumSize(QSize(MIN_WIDTH,MIN_HEIGHT));
	this->setMaximumSize(QSize(MAX_WIDTH,MAX_HEIGHT));
	this->setAcceptDrops(true);

	this->srcFileGroup = new QGroupBox(this);
	this->srcFileGroup->setGeometry(10,10,MIN_WIDTH-20,100);
	this->srcFileGroup->setTitle(u8str("选择源文件"));

	this->srcFileLine = new QLineEdit(this->srcFileGroup);
	this->srcFileLine->setGeometry(20,20,this->srcFileGroup->width()-150,30);
	connect(this->srcFileLine,SIGNAL(textChanged(const QString&)),this,SLOT(sflChanged()));

	this->selectFileBox = new QPushButton(u8str("选择文件"),this->srcFileGroup);
	this->selectFileBox->setGeometry(this->srcFileGroup->width()-120,20,100,30);
	connect(this->selectFileBox,SIGNAL(clicked()),this,SLOT(selectfile()));

	this->exportlabel = new QLabel(u8str("导出文件"),this);
	this->exportlabel->setGeometry(20,120,this->width()-150,20);

	this->exportFileLine = new QLineEdit(this);
	this->exportFileLine->setGeometry(20,150,this->width()-150,30);
	connect(this->exportFileLine,SIGNAL(textEdited(const QString&)),this,SLOT(eflChanged()));

	this->exportFileBox = new QPushButton(u8str("选择文件"),this);
	this->exportFileBox->setGeometry(this->width()-120,150,100,30);
	connect(this->exportFileBox,SIGNAL(clicked()),this,SLOT(selectfile2()));

	this->isFuSrcFileBox = new QCheckBox(u8str("覆盖原文件"),this);
	this->isFuSrcFileBox->setGeometry(20,200,100,20);
	connect(this->isFuSrcFileBox,SIGNAL(clicked()),this,SLOT(checkFuSrcFile()));

	this->keylabel = new QLabel(u8str("密钥:"),this);
	this->keylabel->setGeometry(20,250,100,20);

	this->keyLine = new QLineEdit(this);
	this->keyLine->setEchoMode(QLineEdit::Password);
	this->keyLine->setGeometry(70,250,this->width()-200,30);

	this->enycptButton = new QPushButton(u8str("加密/解密"),this);
	this->enycptButton->setGeometry(this->width()-120,250,100,30);
	connect(this->enycptButton,SIGNAL(clicked()),this,SLOT(exenycpt()));

	this->isShowKey = new QCheckBox("显示密钥",this);
	this->isShowKey->setGeometry(20,280,100,20);
	connect(this->isShowKey,SIGNAL(clicked()),this,SLOT(visKey()));

	this->statusbar = new QStatusBar(this);
	this->statusbar->setGeometry(0,this->height()-30,this->width(),30);
}

// 鼠标拖动事件
void Application::dragEnterEvent(QDragEnterEvent* event) {
	event->acceptProposedAction();  // 放行
} 
void Application::dropEvent(QDropEvent* e) {
	QList<QUrl> urls = e->mimeData()->urls();
	if (urls.isEmpty()) return;
	QString path = urls.first().toLocalFile();
	
	this->srcFileLine->setText(path);
}

void Application::selectfile() {
	QString path = QFileDialog::getOpenFileName(this,u8str("选择文件"),QString(),"All File(*.*)");
	if(!path.isEmpty()) {
		this->srcFileLine->setText(path);
	}
}

void Application::selectfile2() {
	QString path = QFileDialog::getSaveFileName(this,u8str("保存文件"),QString(),"All File(*.*)");
	if(!path.isEmpty()) {
		this->exportFileLine->setText(path);
	}
}

void Application::checkFuSrcFile() {
	this->exportFileLine->setEnabled(!this->isFuSrcFileBox->isChecked());
	this->exportFileBox->setEnabled(!this->isFuSrcFileBox->isChecked());
}

void Application::exenycpt() {
	this->statusbar->showMessage("开始加密/解密");
	this->setDisabled(true);
	// toLocal8bit适配中文路径
	string plainp = this->srcFileLine->text().toLocal8Bit().data();
	string key = this->keyLine->text().toLocal8Bit().data();
	string cipp = this->exportFileLine->text().toLocal8Bit().data();
	if (!QFileInfo(this->srcFileLine->text()).isFile() || (QFileInfo(this->exportFileLine->text()).exists() && QFileInfo(this->exportFileLine->text()).isDir())) {
		this->statusbar->showMessage("出现错误");
		QMessageBox::warning(this,"温馨提示","原文件不存在或导出路径不合法",QMessageBox::Ok);
		this->setDisabled(false);
		return;
	}
	if (plainp.empty() || key.empty()) {  // 没有填写源文件路径和密钥
		this->statusbar->showMessage("出现错误");
		QMessageBox::warning(this,"温馨提示","你没有填写源文件路径和密钥哦",QMessageBox::Ok);
		this->setDisabled(false);
		return;
	}
	if (this->isFuSrcFileBox->isChecked() || plainp == cipp) {
		// 先把处理结果存到临时文件夹，再复制过来删除
		cipp = (QDir::tempPath()+"/ENYCPT").toStdString();  // 临时文件路径
		enycptFile(plainp.c_str(),key.c_str(),cipp.c_str());  
		copyfile(cipp,plainp);  // 复制
		remove(cipp.c_str());  // 删除
	}
	else if (cipp.empty()) {  // 导出路径为空且未勾选覆盖原文件
		this->statusbar->showMessage("出现错误");
		QMessageBox::warning(this,"温馨提示","你没有填写导出文件路径哦",QMessageBox::Ok);
		this->setDisabled(false);
		return;
	}
	else {
		enycptFile(plainp.c_str(),key.c_str(),cipp.c_str());
	}
	this->setDisabled(false);
	this->statusbar->showMessage("加密/解密完毕");
}

void Application::visKey() {
	if (this->isShowKey->checkState()) {
		this->keyLine->setEchoMode(QLineEdit::Normal);
	}
	else {
		this->keyLine->setEchoMode(QLineEdit::Password);
	}
}

void Application::sflChanged() {
	QString text = this->srcFileLine->text();
	// 如果用户没有主动修改路径，自动输入路径
	if (!userChanged) {
		if (text.isEmpty()) {
			this->exportFileLine->setText("");
		}
		else {
			this->exportFileLine->setText(text+".enycpt");
		}
	}
}

// 当用户主动修改路径，就不再自动输入路径了
void Application::eflChanged() {
	QString text = this->exportFileLine->text();
	if (text.isEmpty()) {
		userChanged = false;
	}
	else {
		userChanged = true;
	}
}