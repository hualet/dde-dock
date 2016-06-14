#include "dockitemcontroller.h"
#include "dbus/dbusdockentry.h"
#include "item/appitem.h"
#include "item/placeholderitem.h"

#include <QDebug>

DockItemController *DockItemController::INSTANCE = nullptr;

DockItemController *DockItemController::instance(QObject *parent)
{
    if (!INSTANCE)
        INSTANCE = new DockItemController(parent);

    return INSTANCE;
}

DockItemController::~DockItemController()
{
    qDeleteAll(m_itemList);
}

const QList<DockItem *> DockItemController::itemList() const
{
    return m_itemList;
}

DockItemController::DockItemController(QObject *parent)
    : QObject(parent),
      m_dockInter(new DBusDock(this))
{
    for (auto entry : m_dockInter->entries())
        m_itemList.append(new AppItem(entry));
    m_itemList.append(new PlaceholderItem);

    connect(m_dockInter, &DBusDock::EntryAdded, this, &DockItemController::appItemAdded);
    connect(m_dockInter, &DBusDock::EntryRemoved, this, &DockItemController::appItemRemoved);
}

void DockItemController::appItemAdded(const QDBusObjectPath &path)
{
    qDebug() << "entry add: " << path.path();

    AppItem *item = new AppItem(path);

    m_itemList.append(item);
    emit itemInserted(0, item);
}

void DockItemController::appItemRemoved(const QString &appId)
{
    for (int i(0); i != m_itemList.size(); ++i)
    {
        if (m_itemList[i]->itemType() != DockItem::App)
            continue;

        AppItem *app = static_cast<AppItem *>(m_itemList[i]);
        if (app->appId() != appId)
            continue;

        emit itemRemoved(m_itemList[i]);
        m_itemList[i]->deleteLater();
        m_itemList.removeAt(i);

        break;
    }
}