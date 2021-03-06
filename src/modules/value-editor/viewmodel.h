#pragma once
#include <functional>
#include <QAbstractListModel>
#include <QString>
#include <QPair>
#include <QByteArray>
#include <QSharedPointer>
#include <QJSValue>
#include "abstractkeyfactory.h"
#include "keymodel.h"

namespace ConnectionsTree {
    class KeyItem;
}

namespace ValueEditor {

class ViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        keyNameRole = Qt::UserRole + 1,
        keyDisplayName,
        keyIndex,
        keyTTL,
        keyType,        
        showValueNavigation,
        columnNames,
        count,
        keyValue
    };

public:
    ViewModel(QSharedPointer<AbstractKeyFactory> keyFactory);

    QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;    
    QHash<int, QByteArray> roleNames() const override;


public: // methods exported to QML
    Q_INVOKABLE void addKey(QString keyName, QString keyType,
                            const QVariantMap &row, QJSValue jsCallback);
    Q_INVOKABLE void renameKey(int index, const QString& newKeyName);
    Q_INVOKABLE void removeKey(int i);
    Q_INVOKABLE void closeTab(int i);
    Q_INVOKABLE void setCurrentTab(int i);
    Q_INVOKABLE QObject* getValue(int i);    

signals:
    void keyError(int index, const QString& error);
    void replaceTab(int index);
    void closeWelcomeTab();
    void newKeyDialog(QString dbIdentificationString, QString keyPrefix);

public slots:
    void openNewKeyDialog(QSharedPointer<RedisClient::Connection> connection,
                          std::function<void()>, int dbIndex, QString keyPrefix);
    void openTab(QSharedPointer<RedisClient::Connection> connection,
                 ConnectionsTree::KeyItem& key, bool inNewTab);
    void closeDbKeys(QSharedPointer<RedisClient::Connection> connection, int dbIndex);

private:
    QList<QSharedPointer<Model>> m_valueModels;
    QSharedPointer<AbstractKeyFactory> m_keyFactory;
    int m_currentTabIndex;

    QPair<QSharedPointer<RedisClient::Connection>, int> m_newKeyRequest;
    std::function<void()> m_newKeyCallback;

    bool isIndexValid(const QModelIndex &index) const;
    void loadModel(QSharedPointer<Model> model, bool openNewTab = false);
    void removeModel(QSharedPointer<Model> model);
};

}
