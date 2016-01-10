#pragma once

#include <mutex>

namespace yarrar
{
template <typename T>
class LockableData
{
    class ReadWriteHandle;
    class ReadHandle;

public:
    LockableData(T&& data)
        : m_data(std::move(data))
    {
    }
    ~LockableData() = default;
    LockableData(LockableData&&) = delete;
    LockableData(const LockableData&) = delete;
    LockableData& operator=(const LockableData&) = delete;
    LockableData& operator=(LockableData&&) = delete;

    ReadWriteHandle lockReadWrite()
    {
        return { *this };
    }

    ReadHandle lockRead() const
    {
        return { *this };
    }

private:
    mutable std::mutex m_mutex;
    T m_data;

    // A proxy class to read and write the data.
    // Locks the data structure when alive.
    class ReadWriteHandle
    {
    public:
        ~ReadWriteHandle() = default;
        ReadWriteHandle(ReadWriteHandle&&) = default;
        ReadWriteHandle(const ReadWriteHandle&) = delete;
        ReadWriteHandle& operator=(const ReadWriteHandle&) = delete;
        ReadWriteHandle& operator=(ReadWriteHandle&&) = delete;

        void set(T&& data)
        {
            m_parent.m_data = std::move(data);
        }

        const T& get()
        {
            return m_parent.m_data;
        }

    private:
        friend class LockableData;

        ReadWriteHandle(LockableData& lockable)
            : m_lock(lockable.m_mutex)
            , m_parent(lockable)
        {
        }

        std::unique_lock<std::mutex> m_lock;
        LockableData& m_parent;
    };

    // A proxy class to read the data.
    // Locks the data structure when alive.
    class ReadHandle
    {
    public:
        ~ReadHandle() = default;
        ReadHandle(ReadHandle&&) = default;
        ReadHandle(const ReadHandle&) = delete;
        ReadHandle& operator=(const ReadHandle&) = delete;
        ReadHandle& operator=(ReadHandle&&) = delete;

        const T& get()
        {
            return m_parent.m_data;
        }

    private:
        friend class LockableData;

        ReadHandle(const LockableData& lockable)
            : m_lock(lockable.m_mutex)
            , m_parent(lockable)
        {
        }

        std::unique_lock<std::mutex> m_lock;
        const LockableData& m_parent;
    };
};
}
