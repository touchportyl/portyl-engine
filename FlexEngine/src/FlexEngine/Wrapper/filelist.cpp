#include "pch.h"

#include "filelist.h"

namespace FlexEngine
{

  #pragma region Passthrough to std::vector

  #pragma region Element access

  FileList::reference               FileList::at(size_type index) { return m_files.at(index); }
  FileList::reference               FileList::operator[](size_type index) { return m_files[index]; }
  FileList::reference               FileList::front() { return m_files.front(); }
  FileList::reference               FileList::back() { return m_files.back(); }
  FileList::pointer                 FileList::data() noexcept { return m_files.data(); }

  #pragma endregion

  #pragma region Iterators

  FileList::iterator                FileList::begin() noexcept { return m_files.begin(); }
  FileList::const_iterator          FileList::cbegin() const noexcept { return m_files.cbegin(); }
  FileList::iterator                FileList::end() noexcept { return m_files.end(); }
  FileList::const_iterator          FileList::cend() const noexcept { return m_files.cend(); }
  FileList::reverse_iterator        FileList::rbegin() noexcept { return m_files.rbegin(); }
  FileList::const_reverse_iterator  FileList::crbegin() const noexcept { return m_files.crbegin(); }
  FileList::reverse_iterator        FileList::rend() noexcept { return m_files.rend(); }
  FileList::const_reverse_iterator  FileList::crend() const noexcept { return m_files.crend(); }

  #pragma endregion

  #pragma region Capacity

  bool                              FileList::empty() const noexcept { return m_files.empty(); }
  FileList::size_type               FileList::size() const noexcept { return m_files.size(); }
  FileList::size_type               FileList::max_size() const noexcept { return m_files.max_size(); }
  void                              FileList::reserve(size_type new_capacity) { m_files.reserve(new_capacity); }
  FileList::size_type               FileList::capacity() const noexcept { return m_files.capacity(); }
  void                              FileList::shrink_to_fit() { m_files.shrink_to_fit(); }

  #pragma endregion

  #pragma region Modifiers

  void                              FileList::clear() noexcept { m_files.clear(); }
  FileList::iterator                FileList::insert(const_iterator position, const value_type& value) { return m_files.insert(position, value); }
  FileList::iterator                FileList::insert(const_iterator position, value_type&& value) { return m_files.insert(position, value); }
  FileList::iterator                FileList::insert(const_iterator position, size_type count, const value_type& value) { return m_files.insert(position, count, value); }
  template <typename InputIt>
  FileList::iterator                FileList::insert(const_iterator position, InputIt first, InputIt last) { return m_files.insert(position, first, last); }
  FileList::iterator                FileList::insert(const_iterator position, std::initializer_list<value_type> ilist) { return m_files.insert(position, ilist); }
  template <typename... Args>
  FileList::iterator                FileList::emplace(const_iterator position, Args&&... args) { return m_files.emplace(position, args...); }
  FileList::iterator                FileList::erase(const_iterator position) { return m_files.erase(position); }
  FileList::iterator                FileList::erase(const_iterator first, const_iterator last) { return m_files.erase(first, last); }
  void                              FileList::push_back(const value_type& value) { m_files.push_back(value); }
  void                              FileList::push_back(value_type&& value) { m_files.push_back(value); }
  template <typename... Args>
  FileList::reference               FileList::emplace_back(Args&&... args) { return m_files.emplace_back(args...); }
  void                              FileList::pop_back() { m_files.pop_back(); }
  void                              FileList::resize(size_type count) { m_files.resize(count); }
  void                              FileList::resize(size_type count, const value_type& value) { m_files.resize(count, value); }
  void                              FileList::swap(FileList& other) noexcept { m_files.swap(other.m_files); }

  #pragma endregion

  #pragma region Non-member functions

  bool operator==(const FileList& lhs, const FileList& rhs) noexcept { return lhs.m_files == rhs.m_files; }

  #pragma endregion

  #pragma endregion

  #pragma region Helper functions

  void FileList::each(std::function<void(File&)> func)
  {
    for (auto& path : m_files)
    {
      File& file = FlexEngine::File::Open(path);
      func(file);
    }
  }

  FileList FileList::find_if(std::function<bool(const Path&)> predicate)
  {
    FileList found;

    for (auto& file : m_files)
    {
      if (predicate(file))
      {
        found.push_back(file);
      }
    }

    return found;
  }

  FileList FileList::GetAllFilesInDirectory(const Path& directory)
  {
    // guard
    if (!directory.is_directory())
    {
      Log::Error("Attempted to get files from a non-directory: " + directory.string());
      return {};
    }

    FileList files;

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
      // guard
      if (entry.is_directory()) continue;

      // catch unsafe extensions
      try
      {
        files.push_back({ entry.path() });
      }
      catch (const std::exception& e)
      {
        Log::Warning(e.what());
        continue;
      }
    }

    return files;
  }

  FileList FileList::GetAllFilesInDirectoryRecursively(const Path& directory)
  {
    // guard
    if (!directory.is_directory())
    {
      Log::Error("Attempted to get files from a non-directory: " + directory.string());
      return {};
    }

    FileList files;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
      // guard
      if (entry.is_directory()) continue;

      // catch unsafe extensions
      try
      {
        files.push_back({ entry.path() });
      }
      catch (const std::exception& e)
      {
        Log::Warning(e.what());
        continue;
      }
    }

    return files;
  }

  #pragma endregion

}
