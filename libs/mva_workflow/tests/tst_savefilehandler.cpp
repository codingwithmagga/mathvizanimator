#include <QTest>

#include "savefilehandler.h"

class TestSaveFileHandler : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();
  void init();

  void cleanup();

  void saveFileExists();
  void loadFile();

  void wrongExtension();
  void cantOpenSaveFile();
  void cantOpenLoadFile();

 private:
  SaveFileHandler m_savefile_handler;
  QJsonObject m_test_json_data;
};

void noMessageOutput(QtMsgType, const QMessageLogContext &, const QString &) {}

void TestSaveFileHandler::initTestCase() {
  qInstallMessageHandler(noMessageOutput);

  QJsonObject data1;
  data1["y"] = 100;
  data1["width"] = 200;
  data1["height"] = 300;
  data1["item.color"] = "red";

  QJsonObject data2;
  data2["x"] = 100;
  data2["width"] = 220;
  data2["height"] = 100;
  data1["item.name"] = "Item";

  m_test_json_data["data1"] = data1;
  m_test_json_data["data2"] = data2;
}

void TestSaveFileHandler::init() {
  QDir save_dir = QDir::current();
  save_dir.mkdir("test_savefiles");
  QVERIFY(save_dir.cd("test_savefiles"));
  m_savefile_handler.setSaveDir(save_dir);
}

void TestSaveFileHandler::cleanup() {
  QDir save_dir = m_savefile_handler.saveDir();
  save_dir.setNameFilters(QStringList() << "*.*");
  save_dir.setFilter(QDir::Files);
  foreach (QString dirFile, save_dir.entryList()) {
    save_dir.remove(dirFile);
  }
}

void TestSaveFileHandler::saveFileExists() {
  QVERIFY(m_savefile_handler.saveJSON("saveTest", m_test_json_data));
  QVERIFY(QFileInfo::exists(
      m_savefile_handler.saveDir().absoluteFilePath("saveTest.json")));
}

void TestSaveFileHandler::loadFile() {
  QVERIFY(m_savefile_handler.saveJSON("saveTest", m_test_json_data));

  const QString save_file =
      m_savefile_handler.saveDir().absoluteFilePath("saveTest.json");
  QJsonDocument jsonDoc(m_test_json_data);

  QCOMPARE(m_savefile_handler.loadJSON(QFileInfo(save_file)), jsonDoc);
}

void TestSaveFileHandler::wrongExtension() {
  QCOMPARE(m_savefile_handler.loadJSON(QFileInfo("test.xml")), QJsonDocument{});
}

void TestSaveFileHandler::cantOpenSaveFile() {
  QDir non_existing_dir("random_dir");
  m_savefile_handler.setSaveDir(non_existing_dir);

  QVERIFY(!m_savefile_handler.saveJSON("saveTest", m_test_json_data));
}

void TestSaveFileHandler::cantOpenLoadFile() {
  QCOMPARE(m_savefile_handler.loadJSON(QFileInfo("test.json")),
           QJsonDocument{});
}

QTEST_MAIN(TestSaveFileHandler)
#include "tst_savefilehandler.moc"
