//===- GOT.h --------------------------------------------------------------===//
//
//                     The MCLinker Project
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef MCLD_GOT_H
#define MCLD_GOT_H
#ifdef ENABLE_UNITTEST
#include <gtest.h>
#endif

#include <mcld/LD/LDSection.h>
#include <mcld/MC/MCTargetFragment.h>

namespace mcld
{

class GOT;
class ResolveInfo;

/** \class GOTEntry
 *  \brief The entry of Global Offset Table
 */
class GOTEntry : public MCTargetFragment
{
public:
  explicit GOTEntry(uint64_t pContent, size_t pEntrySize,
                    llvm::MCSectionData* pParent);

  virtual ~GOTEntry();

  uint64_t& getContent()
  { return f_Content; }

  uint64_t getContent() const
  { return f_Content; }

  void setContent(uint64_t pValue)
  { f_Content = pValue; }

  static bool classof(const MCFragment *pFrag)
  { return pFrag->getKind() == llvm::MCFragment::FT_Target; }

  static bool classof(const GOTEntry* pFrag)
  { return true; }

  // Override pure virtual function
  size_t getSize() const
  { return m_EntrySize; }

protected:
  uint64_t f_Content;
  size_t m_EntrySize;
};

/** \class GOT
 *  \brief The Global Offset Table
 */
class GOT
{
protected:
  GOT(LDSection& pSection,
      llvm::MCSectionData& pSectionData,
      size_t pEntrySize);

public:
  virtual ~GOT();

  /// entrySize - the number of bytes per entry
  size_t getEntrySize() const;

  const LDSection& getSection() const
  { return m_Section; }

  llvm::MCSectionData& getSectionData()
  { return m_SectionData; }

  const llvm::MCSectionData& getSectionData() const
  { return m_SectionData; }

public:
  /// reserveEntry - reseve number of pNum of empty entries
  /// Before layout, we scan all relocations to determine if GOT entries are
  /// needed. If an entry is needed, the empty entry is reserved for layout
  /// to adjust the fragment offset. After that, we fill up the entries when
  /// applying relocations.
  virtual void reserveEntry(size_t pNum = 1) = 0;

  /// getEntry - get an empty entry or an exitsted filled entry with pSymbol.
  /// @param pSymbol - the target symbol
  /// @param pExist - ture if a filled entry with pSymbol existed, otherwise false.
  virtual GOTEntry* getEntry(const ResolveInfo& pSymbol, bool& pExist) = 0;

protected:
  LDSection& m_Section;
  llvm::MCSectionData& m_SectionData;
  size_t f_EntrySize;
};

} // namespace of mcld

#endif

