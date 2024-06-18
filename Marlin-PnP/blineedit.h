#ifndef BLINEEDIT_H
#define BLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QList>
#include <QString>
#include <QDebug>

class BLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  explicit BLineEdit(QWidget* parent = Q_NULLPTR);
  ~BLineEdit();

  void saveText();
  void changeSavedLimit(int limit);
  int getSavedLimit();
  const QList<QString> getSavedTextList();
  const QString getSavedText(int index = -1);

private:
  void setTextFromSaved(int index);

protected:
  void keyPressEvent(QKeyEvent *event);

signals:
  void keyArrowPressed(int key);

private:
  QList<QString> saved_text;
  int saved_limit = 10;
  int show_index = 0;
};

#endif // BLINEEDIT_H
