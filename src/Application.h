#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtWidgets/QtWidgets>

class Application : public QWidget {
	Q_OBJECT;
private:
	QString srcFilepath;
	QString exportFilepath;
	bool userChanged;  // 用户是否修改过导出文件路径

	QGroupBox* srcFileGroup;
	QPushButton* selectFileBox;
	QLineEdit* srcFileLine;
	QLabel* exportlabel;
	QPushButton* exportFileBox;
	QLineEdit* exportFileLine;
	QCheckBox* isFuSrcFileBox;
	QLabel* keylabel;
	QLineEdit* keyLine;
	QPushButton* enycptButton;
	QCheckBox* isShowKey;
	QStatusBar* statusbar;
public:
	Application();
	~Application();
	void setupUI();
private slots:
	void selectfile();
	void selectfile2();
	void checkFuSrcFile();
	void exenycpt();
	void visKey();
	void sflChanged();
	void eflChanged();
protected:
	void dragEnterEvent(QDragEnterEvent* e);
	void dropEvent(QDropEvent* e);
};

#endif