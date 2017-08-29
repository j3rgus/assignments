/**
 * Copyright (c) 2000 GEMPLUS group. All Rights Reserved.
 *------------------------------------------------------------------------------
 *  Project name    :  GemXpresso RAD III  Environment
 *                      - Java Card applet -
 *
 *  Platform        :  Java virtual machine
 *  Language        :  java 1.3.1-b24
 *  Devl tool       :  Borland (c) JBuilder 7.0
 *
 *  Original author : Author
 *  Date            : Wed Nov 20 14:39:55 CET 2013
 *------------------------------------------------------------------------------
 */

/*
 * Package name
 */
package mypackage;

/*
 * Imported packages
 */
// specific import for Javacard API access
import java.lang.String;
import javacard.framework.*;
import javacard.security.*;
import javacardx.crypto.*;

public class MyApplet extends javacard.framework.Applet
{
    /**
     * MyApplet default constructor
     * Only this class's install method should create the applet object.
     */

  static Key m_privateKey = null;
  static Key m_publicKey = null;
  static KeyPair m_keyPair = null;
  static Signature m_sign = null;
  static short len = 0;

    protected MyApplet(byte[] buffer, short offset, byte length)
    {
        // data offset is used for application specific parameter.
        // initialization with default offset (AID offset).
        short dataOffset = offset;
        boolean isOP2 = false;

        if(length > 9) {
            // Install parameter detail. Compliant with OP 2.0.1.

            // | size | content
            // |------|---------------------------
            // |  1   | [AID_Length]
            // | 5-16 | [AID_Bytes]
            // |  1   | [Privilege_Length]
            // | 1-n  | [Privilege_Bytes] (normally 1Byte)
            // |  1   | [Application_Proprietary_Length]
            // | 0-m  | [Application_Proprietary_Bytes]

            // shift to privilege offset
            dataOffset += (short)( 1 + buffer[offset]);
            // finally shift to Application specific offset
            dataOffset += (short)( 1 + buffer[dataOffset]);

           // <IF NECESSARY, USE COMMENTS TO CHECK LENGTH >
           // // checks wrong data length
           // if(buffer[dataOffset] !=  <PUT YOUR PARAMETERS LENGTH> )
           //     // return received proprietary data length in the reason
           //     ISOException.throwIt((short)(ISO7816.SW_WRONG_LENGTH + offset + length - dataOffset));

            // go to proprietary data
            dataOffset++;

            // update flag
            isOP2 = true;

        } else {
           // <IF NECESSARY, USE COMMENTS TO CHECK LENGTH >
           // if(length != <PUT YOUR PARAMETERS LENGTH> )
           //     ISOException.throwIt((short)(ISO7816.SW_WRONG_LENGTH + length));
       }

        // <PUT YOUR CREATION ACTION HERE>

        // register this instance
        if (isOP2)
          register(buffer, (short)(offset + 1), (byte)buffer[offset]);
        else
          register();
    }

    /**
     * Method installing the applet.
     * @param bArray the array constaining installation parameters
     * @param bOffset the starting offset in bArray
     * @param bLength the length in bytes of the data parameter in bArray
     */
    public static void install(byte[] bArray, short bOffset, byte bLength) throws ISOException
    {
        /* applet  instance creation */
        new MyApplet (bArray, bOffset, (byte)bLength );
    }

    /**
     * Select method returns true if applet selection is supported.
     * @return boolean status of selection.
     */
    public boolean select()
    {
        // <PUT YOUR SELECTION ACTION HERE>

        // return status of selection
        return true;
    }

    /**
     * Deselect method called by the system in the deselection process.
     */
    public void deselect()
    {

        // <PUT YOUR DESELECTION ACTION HERE>

        return;
    }

    /**
     * Method processing an incoming APDU.
     * @see APDU
     * @param apdu the incoming APDU
     * @exception ISOException with the response bytes defined by ISO 7816-4
     */
    public void process(APDU apdu) throws ISOException
    {
        // get the APDU buffer
        byte[] apduBuffer = apdu.getBuffer();
        short dataLen;

        // ignore the applet select command dispached to the process
        if (selectingApplet())
            return;

        /* Prikaz 11h - pripravy kluce */
        if (apduBuffer[ISO7816.OFFSET_INS] == 0x11) {
          m_privateKey = KeyBuilder.buildKey(KeyBuilder.TYPE_RSA_PRIVATE,KeyBuilder.LENGTH_RSA_512,false);
          m_publicKey = KeyBuilder.buildKey(KeyBuilder.TYPE_RSA_PUBLIC,KeyBuilder.LENGTH_RSA_512,true);
          m_keyPair = new KeyPair(KeyPair.ALG_RSA, (short) m_publicKey.getSize());

          m_keyPair.genKeyPair();

          m_publicKey = m_keyPair.getPublic();
          m_privateKey = m_keyPair.getPrivate();

          m_sign = Signature.getInstance(Signature.ALG_RSA_SHA_PKCS1, false);
          m_sign.init(m_privateKey, Signature.MODE_SIGN);
        }

        /* prikaz 22h - prenos dat */
        if (apduBuffer[ISO7816.OFFSET_INS] == 0x22) {
          dataLen = apdu.setIncomingAndReceive();
          len += dataLen;

          /* zbiera data */
          if (apduBuffer[ISO7816.OFFSET_P1] == 1) {
            m_sign.update(apduBuffer, ISO7816.OFFSET_CDATA, dataLen);
          }

          /* posledne adpu s datami podpise */
          if (apduBuffer[ISO7816.OFFSET_P1] == 2) {
            byte[] out = new byte[64];
            short signLen;

            signLen = m_sign.sign(apduBuffer, (byte) ISO7816.OFFSET_CDATA, len, out, (byte) 0);
            Util.arrayCopyNonAtomic(out, (short) 0, apduBuffer, ISO7816.OFFSET_CDATA, signLen);
            apdu.setOutgoingAndSend(ISO7816.OFFSET_CDATA, signLen);
          }
        }
    }
}

