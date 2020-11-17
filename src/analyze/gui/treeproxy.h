/*
 * Copyright 2015-2017 Milian Wolff <mail@milianw.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TREEPROXY_H
#define TREEPROXY_H

#include <QSortFilterProxyModel>

class TreeProxy final : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TreeProxy(int symbolRole, int resultDataRole, QObject* parent = nullptr);
    virtual ~TreeProxy();

public slots:
    void setFunctionFilter(const QString& functionFilter);
    void setModuleFilter(const QString& moduleFilter);

private:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    const int m_symbolRole;
    const int m_resultDataRole;

    QString m_functionFilter;
    QString m_moduleFilter;
};

#endif // TREEPROXY_H
