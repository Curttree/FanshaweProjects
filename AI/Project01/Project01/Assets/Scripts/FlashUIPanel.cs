using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FlashUIPanel : MonoBehaviour
{
    public Image image;
    private Color newColor;
    private bool isFading;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (isFading && image.color.a > 0)
        {
            newColor = image.color;
            newColor.a -= Time.deltaTime;
            image.color = newColor;
        }
    }

    public void Flash()
    {
        newColor = image.color;
        newColor.a = 0.8f;
        image.color = newColor;
        isFading = true;
    }
}
