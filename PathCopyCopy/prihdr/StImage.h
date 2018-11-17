// StImage.h
// (c) 2013-2018, Charles Lechasseur
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <assert.h>

#include <windows.h>


//
// StImage
//
// Wrapper for a Win32 image handle. Loads the image in constructor
// and takes care of destroying it properly in destructor.
//
class StImage final
{
public:
                        //
                        // Constructor that loads the image with LoadImage.
                        // The image will be destroyed as appropriate in destructor,
                        // unless it was loaded with the LR_SHARED flag.
                        //
                        // @param p_hImageModule Handle of module instance that contains the image resource.
                        //                       To load an image from a file, pass NULL.
                        // @param p_pImageNameOrId When loading an image from a resource, pass either the name
                        //                         of the resource or the resource ID using MAKEINTRESOURCE.
                        //                         When loading an image from a file, pass the name of the file.
                        //                         To load an image from a file, pass LR_LOADFROMFILE in p_LoadFlags.
                        // @param p_ImageType Type of image to load. Use the IMAGE_ macros (IMAGE_BITMAP, etc.)
                        // @param p_DesiredX Width of desired image, in pixels. If set to 0, will use the default system size.
                        // @param p_DesiredY Height of desired image, in pixels. If set to 0, will use the default system size.
                        // @param p_LoadFlags A combination of LR_ macros describing how to load the image.
                        //
                        StImage(HINSTANCE const p_hImageModule,
                                LPCTSTR p_pImageNameOrId,
                                const UINT p_ImageType,
                                const int p_DesiredX,
                                const int p_DesiredY,
                                const UINT p_LoadFlags)
                            : m_hImage(::LoadImage(p_hImageModule, p_pImageNameOrId, p_ImageType, p_DesiredX, p_DesiredY, p_LoadFlags)),
                              m_Type(p_ImageType),
                              m_Shared((p_LoadFlags & LR_SHARED) != 0),
                              m_LoadResult(m_hImage != NULL ? ERROR_SUCCESS : ::GetLastError())
                        {
                        }

                        //
                        // Constructor that assumes ownership of a bitmap handle.
                        // Will be destroyed by destructor unless shared.
                        //
                        // @param p_hImage Image handle. Cannot be NULL.
                        // @param p_ImageType Type of image. Use one of the IMAGE_ macros (IMAGE_BITMAP, etc.)
                        // @param p_Shared Whether bitmap has been loaded with the LR_SHARED flag.
                        //
                        StImage(HANDLE const p_hImage,
                                const UINT p_ImageType,
                                const bool p_Shared)
                            : m_hImage(p_hImage),
                              m_Type(p_ImageType),
                              m_Shared(p_Shared),
                              m_LoadResult(ERROR_SUCCESS)
                        {
                            assert(p_hImage != NULL);
                        }

                        //
                        // Copying not supported.
                        //
                        StImage(const StImage&) = delete;
    StImage&            operator=(const StImage&) = delete;

                        //
                        // Destructor. Takes care of disposing of the image handle if needed.
                        //
                        ~StImage()
                        {
                            if (m_hImage != NULL && !m_Shared) {
                                switch (m_Type) {
                                    case IMAGE_BITMAP: {
                                        ::DeleteObject(static_cast<HGDIOBJ>(m_hImage));
                                        break;
                                    }
                                    case IMAGE_CURSOR: {
                                        ::DestroyCursor(static_cast<HCURSOR>(m_hImage));
                                        break;
                                    }
                                    case IMAGE_ICON: {
                                        ::DestroyIcon(static_cast<HICON>(m_hImage));
                                        break;
                                    }
                                    default:
                                        assert(false);
                                }
                            }
                        }

                        //
                        // Returns the untyped image handle.
                        //
                        // @return Image handle.
                        //
    HANDLE              Get() const
                        {
                            return m_hImage;
                        }

                        //
                        // Returns the bitmap handle. Can only be called
                        // if the image type is IMAGE_BITMAP.
                        //
                        // @return Bitmap handle.
                        //
    HBITMAP             GetBitmap() const
                        {
                            assert(m_Type == IMAGE_BITMAP);
                            return static_cast<HBITMAP>(m_hImage);
                        }

                        //
                        // Returns the cursor handle. Can only be called
                        // if the image type is IMAGE_CURSOR.
                        //
                        // @return Cursor handle.
                        //
    HCURSOR             GetCursor() const
                        {
                            assert(m_Type == IMAGE_CURSOR);
                            return static_cast<HCURSOR>(m_hImage);
                        }

                        //
                        // Returns the icon handle. Can only be called
                        // if the image type is IMAGE_ICON.
                        //
                        // @return Icon handle.
                        //
    HICON               GetIcon() const
                        {
                            assert(m_Type == IMAGE_ICON);
                            return static_cast<HICON>(m_hImage);
                        }

                        //
                        // Returns the type of image stored.
                        //
                        // @return Image type. Corresponds to an IMAGE_ macro (IMAGE_BITMAP, etc.)
                        //
    UINT                GetType() const
                        {
                            return m_Type;
                        }

                        //
                        // Checks if the image handle was loaded with the LR_SHARED flag. If it was,
                        // it will not be disposed of by the destructor; system will take care of it.
                        //
                        // @return true if the image handle was loaded with LR_SHARED
                        //
    bool                IsShared() const
                        {
                            return m_Shared;
                        }

                        //
                        // Returns a result code for the LoadImage call performed by the constructor.
                        // Will return ERROR_SUCCESS if load was successful, otherwise a Win32 error code.
                        //
                        // @return Result code.
                        //
    DWORD               GetLoadResult() const
                        {
                            return m_LoadResult;
                        }

private:
    HANDLE              m_hImage;       // Handle of image.
    UINT                m_Type;         // Type of image.
    bool                m_Shared;       // Whether image has been loader with LR_SHARED.
    DWORD               m_LoadResult;   // Result of LoadImage call, if any.
};
