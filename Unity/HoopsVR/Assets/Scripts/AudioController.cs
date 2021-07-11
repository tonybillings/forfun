using UnityEngine;
using System.Collections;

public class AudioController : MonoBehaviour 
{
    public AudioClip[] Clips;

    private AudioSource _audioSource;

    void Start()
    {
        _audioSource = GetComponent<AudioSource>();
    }

    void OnCollisionEnter(Collision collision)
    {
        _audioSource.clip = Clips[Random.Range(0, Clips.Length)];
        _audioSource.Play();
    }
}
