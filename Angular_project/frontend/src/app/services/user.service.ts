import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class UserService {

  constructor(private http: HttpClient) { }

  uri = "http://localhost:4000/user";

  login(username: string, password: string) {
    let data = {
      username: username, password: password
    }
    return this.http.post(`${this.uri}/login`, data)
  }

  getUser(username: string) {
    let data = {
      username: username
    }
    return this.http.post(`${this.uri}/getUser`, data)
  }

  getAllUsers(){
    return this.http.get(`${this.uri}/getAllUsers`)
  }

  getAllDoctors(){
    return this.http.get(`${this.uri}/getAllDoctors`)
  }

  deleteUser(username: string){
    let data = {
      username: username
    }
    return this.http.post(`${this.uri}/deleteUser`, data)
  }

  addUser(username, password, type, adress, branch, 
    firstname, lastname, licenceNumber, mail,
    phone, specialization, image) {
    let data = {
      username: username,
      password: password,
      type: type,
      adress: adress,
      branch: branch,
      firstname: firstname,
      lastname: lastname,
      licenceNumber: licenceNumber,
      mail: mail,
      phone: phone,
      specialization: specialization,
      image: image
    }
    return this.http.post(`${this.uri}/addUser`, data)
  }

  updatePatient(username, firstname, lastname, mail, phone, adress){
    let data = {
      username: username,
      firstname: firstname,
      lastname: lastname,
      mail: mail,
      phone: phone,
      adress: adress
    }
    return this.http.post(`${this.uri}/updatePatient`, data)
  }

  addImage(username, image){
    let data = {
      username: username,
      image: image
    }
    return this.http.post(`${this.uri}/addImage`, data)
  }

}
