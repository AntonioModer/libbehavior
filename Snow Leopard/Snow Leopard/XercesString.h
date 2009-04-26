/* ========== Beginning of file XercesString.h ========== */
/*!
  \file   XercesString.h
  \brief  Definition of class XercesString.
*/

#ifndef _XMLDB_XERCES_STRING_H_
#  define _XMLDB_XERCES_STRING_H_

// ========== Includes ==========

// ----- standard header -----
#include <ostream>  // operator<<()
#include <string>  // std::string

// ----- xerces header -----
#include <xercesc/util/XMLString.hpp>  // XMLCh*

XERCES_CPP_NAMESPACE_USE


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


// ========== Classes ==========

// ######################################################################
//! Wrapper class for string conversion.
// ----------------------------------------------------------------------
/*!
*/
// ######################################################################
class XercesString {
public:  // --- public methods ---
	// ********************************************************************
	//! Constructor for STL string.
	// --------------------------------------------------------------------
	/*!
		\param pStr  STL string to be used as content. Can be omitted, if epmty.
	*/
	// ********************************************************************
	XercesString( const std::string& str_ = std::string() )
		: m_pXercesCh( 0 ), m_Str( str_ ), m_StrIsValid( true ) {}
	// ********************************************************************
	//! Constructor for xerces-c string.
	// --------------------------------------------------------------------
	/*!
		\param pXmlCh  xerces-c char array to be used as content
		\param pOwner  flag, if the XercesString should take ownership of array 
	*/
	// ********************************************************************
	XercesString( const XMLCh* pXmlCh_, bool owner_ = false )
		: m_pXercesCh( const_cast<XMLCh*>( pXmlCh_ ) ), m_HaveOwnership( owner_ ),
		m_StrIsValid( false ) {}
	// ********************************************************************
	//! Desctructor.
	// ********************************************************************
	~XercesString() {
		if ( m_pXercesCh && m_HaveOwnership ) {
			XMLString::release( &m_pXercesCh );
		}
	}
	// ********************************************************************
	//! Conversion to STL string.
	// --------------------------------------------------------------------
	/*!
		\return  STL string
	*/
	// ********************************************************************
	const std::string& stdStr() const {
		/* ------------------------------------------------------------
		- if the string need to be converted yet
			- create a char array via transcode
			- copy the char array to the string
			- set validity flag
			- release the memory for the temporary char array
		- return the string
		------------------------------------------------------------ */
		if ( ! m_StrIsValid ) {
			char* chStr = XMLString::transcode( m_pXercesCh );
			m_Str = chStr;
			m_StrIsValid = true;
			XMLString::release( &chStr );
		}
		return m_Str;
	}
	// ********************************************************************
	//! Conversion to xerces-c char array.
	// --------------------------------------------------------------------
	/*!
		\return  xerces-c char array (ownership stays in XercesString object)
	*/
	// ********************************************************************
	const XMLCh* xmlCh() const {
		if ( ! m_pXercesCh ) {
			m_pXercesCh = XMLString::transcode( m_Str.c_str() );
			m_HaveOwnership = true;
		}
		return m_pXercesCh;
	}
	operator std::string () const {
		return this->stdStr();
	}
private:  // --- private methods ---
	//! Disable copy constructor.
	XercesString( const XercesString& );
	//! Disable assignment operator.
	const XercesString& operator=( const XercesString& );
private:  // --- private attributes ---
	mutable XMLCh* m_pXercesCh;  //!< xerces-c representation of the string
	mutable bool m_HaveOwnership;  //!< flag to know, if the instance has the ownership of the xerces string
	mutable std::string m_Str;  //!< STL representation of the string
	mutable bool m_StrIsValid;  //!< flag if STL string is initialized yet
};  // ### class XercesString ###

// **********************************************************************
//! Output stream operator.
// ----------------------------------------------------------------------
/*!
	\param pStream  floating stream (input)
	\param pString  string to be output
	\return  floating stream (output)
*/
// **********************************************************************
inline std::ostream& operator<<( std::ostream& stream_,
	const XercesString& string_ ) {
	return stream_ << string_.stdStr();
}  // *** operator<<() ***

// **********************************************************************
//! Output stream operator for XMLCh*.
// ----------------------------------------------------------------------
/*!
	\param pStream  floating stream (input)
	\param pCh  xml string to be output
	\return  floating stream (output)
*/
// **********************************************************************
inline std::ostream& operator<<( std::ostream& stream_,
	const XMLCh* pCh_ ) {
	/* ------------------------------------------------------------
	- return, if the xerces string pointer is invalid
	- create a char array from the xerces string via transcode
	- append the char array to the stream
	- release memory for the char array
	- return the stream
	------------------------------------------------------------ */
	if ( NULL == pCh_ ) {
		return stream_;
	}
	char* pCh = XMLString::transcode( pCh_ );
	stream_ << pCh;
	XMLString::release( &pCh );
	return stream_;
}  // *** operator<<() ***


#endif  // --- #ifndef _XMLDB_XERCES_STRING_H_ ---

/* ========== End of file XercesString.h ========== */